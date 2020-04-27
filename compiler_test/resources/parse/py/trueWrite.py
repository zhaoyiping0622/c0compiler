if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    constInta,
    charb,
    arrayIntc,
    main.setbody(
        ASTWrite(
            args=string1
                .copy(a)
                .copy(b)
                .copy(ASTExpression(operatorType="ARRAY", expression1=c, expression2=a))
                .copy(ASTExpression(operatorType="ADD", expression1=a, expression2=b))
        ).copy(
            ASTWrite(args=a)
        )
    )
])
