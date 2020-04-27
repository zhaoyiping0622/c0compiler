if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    inta,
    charb,
    arrayIntc,
    main.setbody(
        ASTRead(args=a)
            .copy(ASTRead(args=a.copy(b)))
            # .copy(ASTRead(args=ASTExpression(operatorType="ARRAY", expression1=c, expression2=b).copy(a).copy(b)))
    )
])
