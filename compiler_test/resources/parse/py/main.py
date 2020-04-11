if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []

lines.append(ASTDeclareFun(valueId="main", returnType="VOID"))

printLines(lines)
