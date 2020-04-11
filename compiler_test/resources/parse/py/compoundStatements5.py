if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
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
