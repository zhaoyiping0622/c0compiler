if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    constInta.copy(),
    voidFuna.copy().setbody(
        ASTExpression(operatorType="ASSIGN",expression1=a,expression2=num1)
    )
])
