if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    voidFuna.setargs([("INT", "a")]),
    main.setbody(ASTCall(funName="funa"))
])
