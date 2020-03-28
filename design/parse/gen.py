import re


def trans():
    allNonterminate = {}

    def _trans(x):
        if type(x) is re.Match:
            x = x.group(0)
            x = x[1:-1]
        if x in allNonterminate:
            return " " + allNonterminate[x] + " "
        s = "nt" + re.sub("[^a-zA-Z0-9_]", "_", x).upper()
        if s in allNonterminate.values():
            raise Exception(f"{x}:{s} is in use")
        allNonterminate[x] = s
        return " " + s + " "

    return _trans


c2t = {}
with open("character2tokentype")as f:
    lines = f.readlines()
    for line in lines:
        line = line.split()
        c2t[line[0]] = line[1]
terminate = []
nonterminate = []
with open("grammar.parse")as f:
    lines = f.readlines()
    for line in lines:
        if line[0] == 'T':
            terminate.append(line.split()[1])
        elif line[0] == 'N':
            nonterminate.append(line[2:])
        else:
            print(line)
"""
change terminate to uppercase
"""
for t in terminate:
    for id, key in enumerate(nonterminate):
        nonterminate[id] = key.replace(t, " " + t[1:-1].upper() + " ")

"""
modify nonterminate to uppercase
"""
tran = trans()
for id, t in enumerate(nonterminate):
    nonterminate[id] = re.sub("<.*?>", tran, t)

"""

"""
edges = {}
for t in nonterminate:
    t = t.split("::=")
    edges[t[0]] = t[1]
print(len(edges),len(nonterminate))
nonterminate = edges

for c in sorted(c2t, key=lambda x: len(x), reverse=True):
    t = c2t[c]
    for key, value in nonterminate.items():
        nonterminate[key] = value.replace(c, " " + t + " ")
with open('out','w')as f:
    for key,value in nonterminate.items():
        print(key,' '.join(value.rsplit()),file=f)
