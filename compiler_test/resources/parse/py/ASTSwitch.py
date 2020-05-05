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
            .copy(ASTSwitch(expression=aminusb, cases=aplusb))
            .copy(ASTSwitch(expression=aminusb, cases=char1.copy(aplusb)))
            .copy(ASTSwitch(expression=aplusb, cases=num0.copy(aplusb).copy(num1).copy(aminusb)))
            .copy(ASTSwitch(expression=aplusb, cases=aminusb.copy(num0).copy(aplusb).copy(num1).copy(aminusb)))
            .copy(ASTSwitch(expression=a, cases=
        num0
                            .copy(aplusb)
                            .copy(ASTLeaf(value="1", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="2", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="3", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="4", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="5", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="6", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="7", valueType="UNSIGNED"))
                            .copy(aplusb)
                            .copy(ASTLeaf(value="8", valueType="UNSIGNED"))
                            .copy(aplusb)
                            ))
    )
])
