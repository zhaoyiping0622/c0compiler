import re

zhdic = {}
endic = {}
for id, x in enumerate(open('zhkeys').readlines()):
    zhdic[x[:-1]] = id
for id, x in enumerate(open('enkeys').readlines()):
    endic[x[:-1]] = id
p = '<.*?>'
enlines = open('grammar.en').readlines()
zhlines = open('grammar.zh').readlines()

for i in range(len(enlines)):
    zh = []
    en = []
    for x in re.findall(p, zhlines[i]):
        if x in zhdic:
            zh.append(zhdic[x])
    for x in re.findall(p, enlines[i]):
        if x in endic:
            en.append(endic[x])
    if zh != en:
        print(i + 1, zh, en)
