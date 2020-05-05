if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    main.setbody(
        inta
            .copy(intb)
            .copy(arrayIntc.setlength(8))
            .copy(aplusb)
            .copy(aminusb)
            .copy(amulb)
            .copy(ASTExpression(operatorType="DIV", expression1=a, expression2=b))
            .copy(ASTExpression(operatorType="MINUS", expression1=a))
            .copy(ASTCondition(cmp=aLTb, thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=aLEb, thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=aGTb, thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=aGEb, thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=aNEb, thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=aEQb, thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=ASTExpression(operatorType="AND", expression1=aLTb, expression2=aGTb),
                               thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=ASTExpression(operatorType="OR", expression1=aLTb, expression2=aGTb),
                               thenStatements=aplusb, elseStatements=aminusb))
            .copy(ASTCondition(cmp=ASTExpression(operatorType="NOT", expression1=aLTb), thenStatements=aplusb,
                               elseStatements=aminusb))
            .copy(aASSIGNaplusb)
            .copy(ASTExpression(operatorType="ADD",
                                expression1=ASTExpression(operatorType="ARRAY", expression1=c, expression2=a),
                                expression2=b))
            .copy(ASTExpression(operatorType="ADD",expression1=a,expression2=ASTExpression(operatorType="ADD",expression1=numf1,expression2=num1)))
            .copy(ASTExpression(operatorType="ASSIGN",
                                expression1=ASTExpression(operatorType="ARRAY", expression1=c, expression2=a),
                                expression2=b))
    )
])
