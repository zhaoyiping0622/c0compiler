if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []

lines.append(
    ASTCall(
        funName="a",
        args=aplusb
    )
)

lines.append(
    ASTCall(
        funName="b"
    )
)

lines.append(
    ASTCall(
        funName="c",
        args=aplusb.copy(
            aminusb.copy(
                amulb
            )
        )
    )
)

lines.append(
    ASTCall(
        funName="c",
        args=ASTCall(
            funName="b"
        )
    )
)

printLines(lines)
