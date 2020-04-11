from copy import deepcopy

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(ASTStatement(operatorType="NOT", statement1=aNE0))
lines.append(deepcopy(aNE0))
lines.append(ASTStatement(operatorType="NE", statement1=aplusb, statement2=num0))
lines.append(deepcopy(aLTb))
lines.append(
    ASTStatement(
        operatorType="AND", statement2=ASTStatement(operatorType="LE", statement1=aplusb, statement2=num0),
        statement1=ASTStatement(
            operatorType="AND",
            statement1=ASTStatement(operatorType="EQ", statement1=aplusb, statement2=num0),
            statement2=ASTStatement(operatorType="GE", statement1=aplusb, statement2=num0)
        )
    )
)
lines.append(
    ASTStatement(
        operatorType="OR", statement2=ASTStatement(operatorType="NE", statement1=aplusb, statement2=num0),
        statement1=ASTStatement(
            operatorType="OR",
            statement1=ASTStatement(operatorType="GT", statement1=aplusb, statement2=num0),
            statement2=ASTStatement(operatorType="LT", statement1=aplusb, statement2=num0)
        )
    )
)

lines.append(
    ASTStatement(
        operatorType="OR",
        statement1=ASTStatement(
            operatorType="AND",
            statement1=ASTStatement(operatorType="GT",statement1=aplusb,statement2=num0),
            statement2=ASTStatement(operatorType="LT",statement1=aplusb,statement2=num0)
        ),
        statement2=ASTStatement(
            operatorType="AND",
            statement1=ASTStatement(operatorType="NE",statement1=aplusb,statement2=num0),
            statement2=ASTStatement(operatorType="EQ",statement1=aplusb,statement2=num0)
        )
    )
)

lines.append(
    ASTStatement(
        operatorType="OR",
        statement1=ASTStatement(
            operatorType="AND",
            statement1=ASTStatement(
                operatorType="NOT",
                statement1=aNE0
            ),
            statement2=bNE0
        ),
        statement2=ASTStatement(
            operatorType="AND",
            statement1=aNE0,
            statement2=ASTStatement(
                operatorType="NOT",
                statement1=bNE0
            )
        )
    )
)

lines.append(
    ASTStatement(
        operatorType="OR",
        statement1=ASTStatement(
            operatorType="NE",
            statement1=char1,
            statement2=num0
        ),
        statement2=aNE0
    )
)

lines.append(
    ASTStatement(
        operatorType="AND",
        statement1=ASTStatement(
            operatorType="NE",
            statement1=num1,
            statement2=num0
        ),
        statement2=aNE0
    )
)
printLines(lines)
