from json import dumps
if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

a = ASTLeaf(value="a", valueType="ID")
b = ASTLeaf(value="b", valueType="ID")
c = ASTStatement(operatorType="ADD", statement1=a, statement2=b)
print(dumps(c.toJSON()).replace("None", "null"))
