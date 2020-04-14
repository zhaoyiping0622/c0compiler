if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(
    ASTLoop(
        cmp=aLTb,
        body=ASTExpression(
            operatorType="ASSIGN",
            expression1=a,
            expression2=aplusb
        )
    )
)
lines.append(
    ASTLoop(
        cmp=ASTExpression(
            operatorType="NE",
            expression1=num1,
            expression2=num0
        ),
        body=aASSIGNaplusb
    )
)
lines.append(
    ASTLoop(
        cmp=ASTExpression(
            operatorType="NE",
            expression1=num1,
            expression2=num0
        ),
        body=ASTExpression(
            operatorType="UNDEFINED"
        )
    )
)
printLines(lines)
