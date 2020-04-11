from copy import deepcopy
from json import dumps

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(deepcopy(aplusb))  # 1
lines.append(deepcopy(aminusb))  # 2
lines.append(ASTStatement(operatorType="ADD", statement1=aminusb, statement2=c))  # 3
lines.append(ASTStatement(operatorType="MINUS", statement1=aplusb, statement2=c))  # 4
lines.append(deepcopy(amulb))  # 5
lines.append(ASTStatement(operatorType="DIV", statement1=amulb, statement2=c))  # 6
lines.append(ASTStatement(operatorType="ADD", statement1=amulb, statement2=c))  # 7
lines.append(ASTStatement(operatorType="MUL", statement1=c, statement2=aplusb))  # 8
lines.append(ASTStatement(operatorType="ADD", statement1=oneaddone, statement2=numf1))  # 9
lines.append(ASTStatement(operatorType="ADD", statement1=oneaddone, statement2=num1))  # 10
lines.append(
    ASTStatement(operatorType="MINUS", statement1=ASTStatement(operatorType="ADD", statement1=num1, statement2=a),
                 statement2=b))  # 11
lines.append(ASTStatement(operatorType="ADD", statement1=aminusb, statement2=charf1))  # 12
lines.append(ASTStatement(operatorType="ADD", statement1=num1, statement2=Aarray1))  # 13
lines.append(ASTStatement(operatorType="ADD", statement1=num1, statement2=Aarrayaplusb))  # 14

# TODO: add call fun

printLines(lines)

