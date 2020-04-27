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
        ASTExpression(operatorType="ARRAY", expression1=c, expression2=d)
    )
])
