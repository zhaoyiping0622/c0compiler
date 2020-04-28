if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    inta, intb,
    main.setbody(
        ASTSwitch(expression=ASTExpression(operatorType="UNDEFINED", expression1=a),
                  cases=aplusb.copy(num0).copy(aplusb).copy(num1).copy(aplusb))
            .copy(ASTSwitch(expression=aplusb, cases=aplusb.copy(num0).copy(aplusb).copy(num1).copy(aplusb)))

    )
])
