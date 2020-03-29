#!/usr/bin/python3
import argparse


def tran(s):
    s = s.replace('\\', '\\\\')
    s = s.replace('"', '\\"')
    return s


def run(file, output):
    with open(file) as f:
        s = f.readlines()
    out = ""
    out += "//@startdot\n"
    out += f"digraph {file.split('/')[-1].split('.')[0]} {{\n"
    out += 'rankdir="LR"\n'
    for line in s:
        line = line.split()
        if len(line) == 2:
            out += f'{line[0]}[shape="doublecircle"]\n'
        else:
            out += f'{line[0]}->{line[1]}[label="{tran(" ".join([line[i] for i in range(2, len(line))]))}"]\n'
    out += "}\n"
    out += "//@enddot\n"
    with open(output, "w") as f:
        f.write(out)


program = argparse.ArgumentParser()
program.add_argument("filename")
program.add_argument("-o", "--output", help="output")
program = program.parse_args()

file = program.filename
output = program.output
run(file, output)
