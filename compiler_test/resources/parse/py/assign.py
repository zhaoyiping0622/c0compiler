from json import dumps
if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines=[]
lines.append(ASTStatement(operatorType="ASSIGN", statement1=a, statement2=b))
lines.append(ASTStatement(operatorType="ASSIGN", statement1=a, statement2=oneaddone))
lines.append(ASTStatement(operatorType="ASSIGN", statement1=a, statement2=bmulc))
lines.append(ASTStatement(operatorType="ASSIGN", statement1=Aarrayb, statement2=c))
lines.append(ASTStatement(operatorType="ASSIGN", statement1=Aarrayb, statement2=num1))

printLines(lines)

