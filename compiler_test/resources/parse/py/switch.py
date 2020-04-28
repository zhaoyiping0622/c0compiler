if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(
    ASTSwitch(
        expression=a,
        cases=ASTExpression(
            operatorType="UNDEFINED",
            next=num1.copy(
                next=aASSIGNaplusb.copy(
                    next=num0.copy(
                        next=bASSIGNaplusb
                    )
                )
            )
        )
    )
)
lines.append(
    ASTSwitch(
        expression=aplusb,
        cases=char1.copy(
            next=aASSIGNaplusb.copy(
                next=char0.copy(
                    next=bASSIGNaplusb
                )
            )
        )
    )
)
lines.append(
    ASTSwitch(
        expression=a,
        cases=ASTExpression(operatorType="UNDEFINED")
    )
)
lines.append(
    ASTSwitch(
        expression=a
    )
)
printLines(lines)
