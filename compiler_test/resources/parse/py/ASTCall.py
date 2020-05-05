if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    intFunb.setbody(
        inta
            .copy(intb)
            .copy(ASTRet(value=aplusb))
    ),
    charFuna
        .setargs([("INT", "a"), ("CHAR", "b")])
        .setbody(
        ASTRet(value=aminusb)
    ),
    main.setbody(
        inta
            .copy(intb)
            .copy(ASTCall(funName="funb"))
            .copy(ASTCall(funName="funa", args=aplusb.copy(aminusb)))
    )
])
