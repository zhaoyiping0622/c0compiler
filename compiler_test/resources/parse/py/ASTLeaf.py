if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    main.setbody(
        inta
            .copy(charb)
            .copy(constIntc.setvalue("1"))
            .copy(constChard.setvalue("'1'"))
            .copy(aplusb)
            .copy(ASTExpression(operatorType="ADD", expression1=b, expression2=c))
            .copy(ASTExpression(operatorType="ADD", expression1=a, expression2=d))
    )
])
