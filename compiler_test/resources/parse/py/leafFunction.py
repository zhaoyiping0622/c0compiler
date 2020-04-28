if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    charFuna,
    main.setbody(ASTLeaf(value="funa",valueType="ID"))
])
