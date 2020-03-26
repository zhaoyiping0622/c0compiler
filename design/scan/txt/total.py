#!/usr/bin/python
from os import listdir
from os import remove

totalnodes = []
totaledges = []


def tran(num, cnt) -> int:
    num = int(num)
    if num == 1:
        return num
    return num + cnt


def run(file, cnt) -> int:
    cc = 0
    global totalnodes
    global totaledges
    with open(file) as f:
        s = f.readlines()
    for line in s:
        line = line.split()
        if len(line) < 2:
            print(f"error {file}")
            continue
        if len(line) == 2:
            totalnodes.append(f"{tran(line[0], cnt)} {line[1]}")
        else:
            totaledges.append(
                f"{tran(line[0], cnt)} {tran(line[1], cnt)} {' '.join([line[i] for i in range(2, len(line))])}")
            cc = max(cc, int(line[0]), int(line[1]))
    return cc


remove("total")
cnt = 0
for file in listdir('.'):
    if file[-2:] != 'py':
        cnt += run(file, cnt) - 1
with open("total", "w")as f:
    f.write("\n".join(totalnodes) + '\n')
    f.write("\n".join(totaledges) + '\n')
