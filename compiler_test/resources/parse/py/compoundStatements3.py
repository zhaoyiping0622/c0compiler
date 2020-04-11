if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(ASTDeclareValue(
    isConst=True,
    valueId="a",
    value="1",
    valueType="INT"
))
lines.append(ASTDeclareValue(
    isConst=True,
    valueId="a",
    value="'1'",
    valueType="CHAR"
).copy(
    ASTDeclareValue(
        isConst=True,
        valueType="CHAR",
        valueId="b",
        value="'a'"
    )
))
lines.append(ASTDeclareValue(
    isConst=True,
    valueId="a",
    value="1",
    valueType="INT"
).copy(
    ASTDeclareValue(
        isConst=True,
        valueId="b",
        value="0",
        valueType="INT"
    ).copy(
        ASTDeclareValue(
            isConst=True,
            valueId="c",
            value="-1",
            valueType="INT"
        )
    )
))
lines.append(
    ASTCondition(
        cmp=aNE0,
        thenStatements=aASSIGNaplusb,
        elseStatements=bASSIGNaplusb
    )
)
lines.append(
    ASTLoop(
        cmp=aNE0,
        body=ASTStatement(
            operatorType="ASSIGN",
            statement1=a,
            statement2=ASTStatement(
                operatorType="ADD",
                statement1=a,
                statement2=ASTCall(funName="b")
            )
        )
    )
)

printLines(lines)
