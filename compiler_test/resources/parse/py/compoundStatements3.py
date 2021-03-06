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
        body=ASTExpression(
            operatorType="ASSIGN",
            expression1=a,
            expression2=ASTExpression(
                operatorType="ADD",
                expression1=a,
                expression2=ASTCall(funName="b")
            )
        )
    )
)

printLines(lines)
