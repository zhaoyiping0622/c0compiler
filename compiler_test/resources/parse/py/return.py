if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []

lines.append(ASTRet(value=num1))
lines.append(ASTRet(value=char1))
lines.append(ASTRet(value=aplusb))
lines.append(ASTRet(value=ASTCall(funName="a")))

printLines(lines)
