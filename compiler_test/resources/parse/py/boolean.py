from copy import deepcopy

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(ASTExpression(operatorType="NOT", expression1=aNE0))
lines.append(deepcopy(aNE0))
lines.append(ASTExpression(operatorType="NE", expression1=aplusb, expression2=num0))
lines.append(deepcopy(aLTb))
lines.append(
    ASTExpression(
        operatorType="AND", expression2=ASTExpression(operatorType="LE", expression1=aplusb, expression2=num0),
        expression1=ASTExpression(
            operatorType="AND",
            expression1=ASTExpression(operatorType="EQ", expression1=aplusb, expression2=num0),
            expression2=ASTExpression(operatorType="GE", expression1=aplusb, expression2=num0)
        )
    )
)
lines.append(
    ASTExpression(
        operatorType="OR", expression2=ASTExpression(operatorType="NE", expression1=aplusb, expression2=num0),
        expression1=ASTExpression(
            operatorType="OR",
            expression1=ASTExpression(operatorType="GT", expression1=aplusb, expression2=num0),
            expression2=ASTExpression(operatorType="LT", expression1=aplusb, expression2=num0)
        )
    )
)

lines.append(
    ASTExpression(
        operatorType="OR",
        expression1=ASTExpression(
            operatorType="AND",
            expression1=ASTExpression(operatorType="GT", expression1=aplusb, expression2=num0),
            expression2=ASTExpression(operatorType="LT", expression1=aplusb, expression2=num0)
        ),
        expression2=ASTExpression(
            operatorType="AND",
            expression1=ASTExpression(operatorType="NE", expression1=aplusb, expression2=num0),
            expression2=ASTExpression(operatorType="EQ", expression1=aplusb, expression2=num0)
        )
    )
)

lines.append(
    ASTExpression(
        operatorType="OR",
        expression1=ASTExpression(
            operatorType="AND",
            expression1=ASTExpression(
                operatorType="NOT",
                expression1=aNE0
            ),
            expression2=bNE0
        ),
        expression2=ASTExpression(
            operatorType="AND",
            expression1=aNE0,
            expression2=ASTExpression(
                operatorType="NOT",
                expression1=bNE0
            )
        )
    )
)

lines.append(
    ASTExpression(
        operatorType="OR",
        expression1=ASTExpression(
            operatorType="NE",
            expression1=char1,
            expression2=num0
        ),
        expression2=aNE0
    )
)

lines.append(
    ASTExpression(
        operatorType="AND",
        expression1=ASTExpression(
            operatorType="NE",
            expression1=num1,
            expression2=num0
        ),
        expression2=aNE0
    )
)
printLines(lines)
