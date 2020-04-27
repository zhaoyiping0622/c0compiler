if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

a = ASTLeaf(value="a", valueType="ID")
b = ASTLeaf(value="b", valueType="ID")
c = ASTExpression(operatorType="ADD", expression1=a, expression2=b)
printLines([c])
