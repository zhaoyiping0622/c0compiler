if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

printLines([
    ASTDeclareFun(valueId="main",returnType="VOID")
])
