if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    intFuna.setbody(ASTRet(value=num1).copy(ASTRet(value=char0))),
    charFunb.setbody(ASTRet(value=num1).copy(ASTRet(value=char0)))
])
