from json import dumps

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(ASTExpression(operatorType="ASSIGN", expression1=a, expression2=b))
lines.append(ASTExpression(operatorType="ASSIGN", expression1=a, expression2=oneaddone))
lines.append(ASTExpression(operatorType="ASSIGN", expression1=a, expression2=bmulc))
lines.append(ASTExpression(operatorType="ASSIGN", expression1=Aarrayb, expression2=c))
lines.append(ASTExpression(operatorType="ASSIGN", expression1=Aarrayb, expression2=num1))

printLines(lines)
