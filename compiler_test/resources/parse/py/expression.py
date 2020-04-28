from copy import deepcopy
from json import dumps

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(deepcopy(aplusb))  # 1
lines.append(deepcopy(aminusb))  # 2
lines.append(ASTExpression(operatorType="ADD", expression1=aminusb, expression2=c))  # 3
lines.append(ASTExpression(operatorType="MINUS", expression1=aplusb, expression2=c))  # 4
lines.append(deepcopy(amulb))  # 5
lines.append(ASTExpression(operatorType="DIV", expression1=amulb, expression2=c))  # 6
lines.append(ASTExpression(operatorType="ADD", expression1=amulb, expression2=c))  # 7
lines.append(ASTExpression(operatorType="MUL", expression1=c, expression2=aplusb))  # 8
lines.append(ASTExpression(operatorType="ADD", expression1=oneaddone, expression2=numf1))  # 9
lines.append(ASTExpression(operatorType="ADD", expression1=oneaddone, expression2=num1))  # 10
lines.append(
    ASTExpression(operatorType="MINUS", expression1=ASTExpression(operatorType="ADD", expression1=num1, expression2=a),
                  expression2=b))  # 11
lines.append(ASTExpression(operatorType="ADD", expression1=aminusb, expression2=charf1))  # 12
lines.append(ASTExpression(operatorType="ADD", expression1=num1, expression2=Aarray1))  # 13
lines.append(ASTExpression(operatorType="ADD", expression1=num1, expression2=Aarrayaplusb))  # 14

# TODO: add call fun

printLines(lines)
