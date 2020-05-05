if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    main.setbody(
        inta
            .copy(charb)
            .copy(ASTWrite(args=a.copy(b).copy(ASTLeaf(value="\"a\"", valueType="STRING"))))
    )
])
