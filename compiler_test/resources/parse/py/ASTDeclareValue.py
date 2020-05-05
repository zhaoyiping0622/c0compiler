if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    ASTDeclareValue(valueId="a", isConst=True, valueType="INT", value="1"),
    ASTDeclareValue(valueId="b", isConst=True, valueType="CHAR", value="'a'"),
    ASTDeclareValue(valueId="c", valueType="INT"),
    ASTDeclareValue(valueId="d", valueType="CHAR"),
    main.setbody(
        ASTDeclareValue(valueId="a", isConst=True, valueType="INT", value="1")
            .copy(ASTDeclareValue(valueId="b", isConst=True, valueType="CHAR", value="'a'"))
            .copy(ASTDeclareValue(valueId="c", valueType="INT"))
            .copy(ASTDeclareValue(valueId="d", valueType="CHAR"))
    )
])
