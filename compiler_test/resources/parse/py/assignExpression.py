if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    intc,
    intd,
    main.setbody(
        ASTExpression(operatorType="ASSIGN", expression1=aplusb, expression2=a)
    )
])
