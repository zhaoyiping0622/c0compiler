if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(
    ASTCondition(
        cmp=aLTb,
        thenStatements=aASSIGNaplusb
    )
)
lines.append(
    ASTCondition(
        cmp=aLTb,
        thenStatements=aASSIGNaplusb,
        elseStatements=bASSIGNaplusb
    )
)
lines.append(
    ASTCondition(
        cmp=aLTb,
        thenStatements=ASTCondition(
            cmp=ASTExpression(
                operatorType="EQ",
                expression1=aplusb,
                expression2=num0
            ),
            thenStatements=aASSIGNaplusb,
            elseStatements=bASSIGNaplusb
        )
    )
)
printLines(lines)
