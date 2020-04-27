if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    constInta.copy(),
    voidFuna.copy().setbody(
        ASTRead(args=ASTLeaf(value="a",valueType="ID"))
    )
])
