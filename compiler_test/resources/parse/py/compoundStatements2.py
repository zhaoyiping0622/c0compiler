if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(
    ASTDeclareValue(
        valueId="a",
        valueType="INT",
    )
)
lines.append(
    ASTDeclareArray(
        valueId="a",
        valueType="CHAR",
        length=1
    ).copy(
        ASTDeclareValue(
            valueId="b",
            valueType="CHAR"
        )
    )
)
lines.append(
    ASTDeclareValue(
        valueId="a",
        valueType="INT"
    ).copy(ASTDeclareArray(
        valueId="b",
        valueType="INT",
        length=1
    ).copy(
        ASTDeclareValue(
            valueId="c",
            valueType="INT"
        )
    ))
)
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
