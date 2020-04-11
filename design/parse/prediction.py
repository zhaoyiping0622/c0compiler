from collections import deque

with open('tmpout') as f:
    s = f.readlines()
rules = {}
for line in s:
    nt = line.split()[0]
    line = line[line.index(' ') + 1:].split('|')
    rules[nt] = list(map(lambda x: x.split(), line))
first = dict(zip(rules.keys(), (set() for i in range(len(rules)))))
follow = dict(zip(rules.keys(), (set() for i in range(len(rules)))))
follow['ntPROGRAM'].add('$')
for key, rule in rules.items():
    rules[key] = dict(
        zip(map(lambda x: ' '.join(x), rule), (set() for i in range(len(rule)))))
dq = deque()
dq.extend(rules.keys())
father = dict(zip(rules.keys(), (set() for i in range(len(rules)))))
while len(dq) != 0:
    now = dq.popleft()
    rule = rules[now]
    changed = False
    for key in rule.keys():
        NULL = True
        skey = key.split()
        for x in skey:
            if x.isupper() or x == 'ntNULL':
                rules[now][key].add(x)
                if x not in first[now]:
                    first[now].add(x)
                    changed = True
                if x == 'ntNULL':
                    NULL = True
                else:
                    NULL = False
                break
            father[x].add(now)
            if "ntNULL" not in first[x]:
                NULL = False
            for f in first[x]:
                rules[now][key].add(f)
                if f not in first[now]:
                    changed = True
                    first[now].add(f)
            if not NULL:
                break
        if NULL:
            if 'ntNULL' not in first[now]:
                first[now].add('ntNULL')
                changed = True
        if changed:
            for x in father[now]:
                dq.append(x)
dq.extend(rules.keys())
while len(dq) != 0:
    now = dq.popleft()
    for key in rules[now].keys():
        skey = key.split()
        for id1, k1 in enumerate(skey):
            if k1.isupper() or k1 == 'ntNULL': continue
            NULL = True
            changed = False
            for id2, k2 in enumerate(skey):
                if id2 <= id1: continue
                if not NULL: break
                if k2.isupper():
                    if k2 not in follow[k1]:
                        changed = True
                        follow[k1].add(k2)
                    NULL = False
                    break
                if k2 == 'ntNULL':
                    NULL = True
                    break
                for t in first[k2]:
                    if t not in follow[k1]: changed = True;follow[k1].add(t)
                if 'ntNULL' not in first[k2]: NULL = False
            if NULL:
                for t in follow[now]:
                    if t not in follow[k1]: changed = True;follow[k1].add(t)
            if changed:
                dq.append(k1)
# for key, value in rules.items():
#     print(key, value)
# for key, value in follow.items():
#     print(key, sorted(list(value)))
# print the intersection of first set
for key, value in rules.items():
    flag = False
    for i, x in enumerate(value.values()):
        for j, y in enumerate(value.values()):
            if i == j: continue
            if len(x.intersection(y)) != 0:
                flag = True
    if flag:
        print(key)
        for k, v in value.items():
            print(f"'{k}'", v)
        print()
