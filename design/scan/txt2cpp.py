#!/usr/bin/python3
import argparse


def getrange(x: str):
    if x == '-':
        return "'-'"
    x = x.split('-')
    if len(x) == 1:
        x = x[0]
        if len(x) == 1:  # char
            if x in '\\\'':
                x = '\\' + x
            return f"'{x}'"
        elif x[:2] == "0x":
            return x
        elif x == "EOF":
            return x
        else:
            print(x)
    else:
        if len(x[0]) >= 2 and x[0][:2] == "0x":
            return (x[0], x[1])
        elif len(x[0]) == 1 and len(x[1]) == 1:
            return (f"'{x[0]}'", f"'{x[1]}'")
        else:
            return None


def run(file, output):
    codes = []
    with open(file) as f:
        s = f.readlines()
    for line in s:
        line = line.split()
        if len(line) == 2:
            codes.append(f"states[{line[0]}].terminate=true;")
            codes.append(f"states[{line[0]}].tokentype={line[1]};")
        else:
            f = line[0]
            t = line[1]
            for i in range(2, len(line)):
                c = getrange(line[i])
                if type(c) is str:
                    codes.append(f"add({f},{t},{c});")
                elif type(c) is tuple:
                    codes.append(
                        f"for(int i={c[0]};i<={c[1]};i++)add({f},{t},i);")
                else:
                    print(line[i])
    with open(output, "w")as f:
        f.write("\n".join(codes))


program = argparse.ArgumentParser()
program.add_argument("filename")
program.add_argument("-o", "--output", help="output")
program = program.parse_args()

file = program.filename
output = program.output
run(file, output)
