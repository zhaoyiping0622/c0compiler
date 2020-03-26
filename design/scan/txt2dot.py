#!/usr/bin/python3
from os import listdir


def tran(s):
    s = s.replace('\\', '\\\\')
    s = s.replace('"', '\\"')
    return s


def run(file, filename):
    with open(file) as f:
        s = f.readlines()
    out = ""
    out += "//@startdot\n"
    out += f"digraph {filename} {{\n"
    out += 'rankdir="LR"\n'
    for line in s:
        line = line.split()
        if len(line) == 2:
            out += f'{line[0]}[shape="doublecircle"]\n'
        else:
            out += f'{line[0]}->{line[1]}[label="{tran(" ".join([line[i] for i in range(2, len(line))]))}"]\n'
    out += "}\n"
    out += "//@enddot\n"
    with open(f"dot/{filename}.dot", "w") as f:
        f.write(out)


for file in listdir("txt"):
    if file[-2:] != 'py':
        run("txt/" + file, file)
