#!/usr/bin/python3
import argparse
from string import digits, ascii_lowercase


def run(file: str, output: str):
    dic = {
        "+": "ADD",
        "-": "MINUS",
        "*": "MUL",
        "/": "DIV",
        "<": "LT",
        "<=": "LE",
        ">": "GT",
        ">=": "GE",
        "!=": "NE",
        "==": "EQ",
        "!": "NOT",
        "||": "OR",
        "&&": "AND",
        ";": "SEMICOLON",
        "(": "LSBRACKETS",
        ")": "RSBRACKETS",
        "{": "LBBRACKETS",
        "}": "RBBRACKETS",
        "[": "LMBRACKETS",
        "]": "RMBRACKETS",
        "=": "ASSIGN",
        ",": "COMMA",
        ":": "COLON",
        "character": "CHARACTER",
        "string": "STRING",
        "unsigned": "UNSIGNED",
        "int": "INT",
        "char": "CHAR",
        "void": "VOID",
        "if": "IF",
        "while": "WHILE",
        "switch": "SWITCH",
        "case": "CASE",
        "scanf": "SCANF",
        "printf": "PRINTF",
        "return": "RETURN",
        "main": "MAIN",
        "default": "DEFAULT",
        "const": "CONST",
        "else": "ELSE"
    }
    inputFile = open(file, "r")
    outputFile = open(output, "w")
    assert not inputFile.closed
    s = '\n'.join(map(lambda x: x.strip(), inputFile.readlines()))
    idx = 0
    while idx < len(s):
        now = ""
        token = None
        if str.isspace(s[idx]):
            idx += 1
            continue
        if idx == len(s) - 1:
            now = s[idx]
            if now in ascii_lowercase:
                token = "ID"
            elif now in digits:
                token = "UNSIGNED"
            else:
                assert now in dic
                token = dic[s[idx]]
            idx += 1
        elif s[idx] == "'":  # character
            assert idx + 2 < len(s)
            now = s[idx:idx + 3]
            token = "CHARACTER"
            idx += 3
        elif s[idx] == '"':  # string
            assert idx + 2 < len(s)
            now = s[idx:idx + 3]
            token = "STRING"
            idx += 3
        elif s[idx] in digits:  # unsigned
            assert idx + 1 == len(s) or s[idx + 1] not in digits,f"{s[idx]},{s[idx+1]}"
            now = s[idx]
            token = "UNSIGNED"
            idx += 1
        elif s[idx] in ascii_lowercase:
            while s[idx] in ascii_lowercase and idx < len(s):
                now += s[idx]
                idx += 1
            if len(now) == 1:  # id
                token = "ID"
            elif now in dic:
                token = dic[now]
            else:
                assert False
        else:
            if s[idx:idx + 2] in dic:
                now = s[idx:idx + 2]
                token = dic[now]
                idx += 2
            else:
                assert s[idx] in dic, f"{s[idx]}"
                now = s[idx]
                token = dic[now]
                idx += 1
        print(token, now, sep="\n", file=outputFile)


program = argparse.ArgumentParser()
program.add_argument("filename")
program.add_argument("-o", "--output", help="output", default="tmp")
program = program.parse_args()

file = program.filename
output = program.output
run(file, output)
