if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines=[]
lines.append(
    ASTLoop(
        cmp=aLTb,
        body=ASTStatement(
            operatorType="ASSIGN",
            statement1=a,
            statement2=aplusb
        )
    )
)
lines.append(
    ASTLoop(
        cmp=ASTStatement(
            operatorType="NE",
            statement1=num1,
            statement2=num0
        ),
        body=aASSIGNaplusb
    )
)
lines.append(
    ASTLoop(
        cmp=ASTStatement(
            operatorType="NE",
            statement1=num1,
            statement2=num0
        ),
        body=ASTStatement(
            operatorType="UNDEFINED"
        )
    )
)
printLines(lines)
