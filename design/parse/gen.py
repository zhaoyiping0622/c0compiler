import re
from collections import deque


class Generate(object):
    def gendot(self, dotFile="graph.dot"):

        dotnodes = []
        dotedges = []

        for key, value in self.nonterminate.items():
            # print(key, list(map(lambda x: x.strip(), value.split('|'))))
            # dotnodes.append(f'{key}[shape="circle"]')
            for id, x in enumerate(value.split('|')):
                s = key.strip() + "__" + str(id + 1)
                dotedges.append(f'{key}->{s}')
                x = x.split()
                dotnodes.append(
                    f'{s}[label="{"|".join(["<f" + str(id) + ">" + xx for id, xx in enumerate(x)])}",shape="record"]')
                for id, xx in enumerate(x):
                    if xx[:2] == "nt":
                        dotedges.append(f"{s}:f{id}->{xx}")
        dot = "digraph g{\n"
        for x in dotnodes:
            dot += x + "\n"
        for x in dotedges:
            dot += x + "\n"
        dot += "}\n"
        with open(dotFile, "w")as f:
            f.write(dot)

    def trans(self, s):
        if type(s) is re.Match:
            s = s.group(0)
            s = s[1:-1]
        if s in self.allNonterminate:
            return f" {self.allNonterminate[s]} "
        ss = f'nt{re.sub("[^a-zA-Z0-9_]", "_", s).upper()}'
        if s in self.allNonterminate.values():
            raise Exception(f"{s}:{ss} is in use")
        self.allNonterminate[s] = ss
        return f" {ss} "

    def inittrans(self):
        self.allNonterminate = {}

    def init_c2t(self, c2tFile):
        self.c2t = {}
        with open(c2tFile) as f:
            lines = f.readlines()
            for line in lines:
                line = line.split()
                self.c2t[line[0]] = line[1]

    def parse_grammar(self, grammarFile):
        self.terminate = []
        self.nonterminate = []
        with open(grammarFile) as f:
            lines = f.readlines()
            for line in lines:
                if line[0] == 'T':
                    self.terminate.append(line.split()[1])
                elif line[0] == 'N':
                    self.nonterminate.append(' '.join(line.split()[1:]))
                else:
                    print(line)
                    exit(1)
        """
        change terminate to uppercase
        and remove '<' '>'
        """
        for t in self.terminate:
            for id, key in enumerate(self.nonterminate):
                self.nonterminate[id] = key.replace(t, " " + t[1:-1].upper() + " ")
        """
        modify nonterminate to uppercase
        """
        self.nonterminate = list(map(lambda x: re.sub("<.*?>", self.trans, x), self.nonterminate))
        """
        modify nonterminate to nonterminate and rules
        """
        tmp = map(lambda x: x.split('::='), self.nonterminate)
        self.nonterminate = {}
        for x in tmp: self.nonterminate[x[0].strip()] = x[1].strip()
        """
        change all character to uppercase
        """
        for c in sorted(self.c2t.keys(), key=lambda x: len(x), reverse=True):
            t = self.c2t[c]
            for key, value in self.nonterminate.items():
                self.nonterminate[key] = value.replace(c, " " + t + " ")
        # self.terminate = list(map(lambda x: x.strip()[1:-1].upper(), self.terminate))
        # self.terminate.extend(self.c2t.values())
        self.terminate = list(set(self.terminate))

    def output(self, filename="tmpout"):
        f = open(filename, "w")
        print("terminate", file=f)
        for x in self.terminate:
            print(x, file=f)
        print("nonterminate", file=f)
        for key, value in self.nonterminate.items():
            print(key, value, file=f)
        f.close()

    def genFirst(self):
        self.first = dict(zip(self.rules.keys(), (set() for i in range(len(self.rules)))))
        """
        rules[x][y] the y-th rule for nonterminate x
        """
        for key, rule in self.rules.items():
            self.rules[key] = dict(zip(rule, (set() for i in range(len(rule)))))
        dq = deque()
        dq.extend(self.rules.keys())
        father = dict(zip(self.rules.keys(), (set() for i in range(len(self.rules)))))
        while len(dq) != 0:
            now = dq.popleft()
            rule = self.rules[now]
            changed = False
            for key in rule.keys():
                NULL = True
                skey = key.split()
                for x in skey:
                    if x in self.c2t.values() or x == 'ntNULL':
                        self.rules[now][key].add(x)
                        if x not in self.first[now]:
                            self.first[now].add(x)
                            changed = True
                        if x == 'ntNULL':
                            NULL = True
                        else:
                            NULL = False
                        break
                    father[x].add(now)
                    if 'ntNULL' not in self.first[x]:
                        NULL = False
                    for f in self.first[x]:
                        self.rules[now][key].add(f)
                        if f not in self.first[now]:
                            changed = True
                            self.first[now].add(f)
                    if not NULL:
                        break
                if NULL:
                    if 'ntNULL' not in self.first[now]:
                        self.first[now].add('ntNULL')
                        change = True
                if changed:
                    dq.extend(father[now])

    def simplifyFirst(self):
        pass

    def genFollow(self):
        self.follow = dict(zip(self.rules.keys(), (set() for i in range(len(self.rules)))))
        self.follow['ntPROGRAM'].add('$')
        pass

    def __init__(self, c2tFile="character2tokentype", grammarFile="grammar.parse"):
        self.init_c2t(c2tFile)
        self.inittrans()
        self.parse_grammar(grammarFile)
        self.rules = {}
        for k, v in self.nonterminate.items():
            self.rules[k] = v.split('|')
        self.genFirst()
        self.output()
        # for k, v in self.nonterminate.items():
        #     print(k, v)


g = Generate()
g.output()
