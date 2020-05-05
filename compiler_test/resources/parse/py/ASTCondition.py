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
            .copy(ASTCondition(cmp=aLTb, thenStatements=aASSIGNaplusb, elseStatements=bASSIGNaplusb))
            .copy(ASTCondition(cmp=aLTb, thenStatements=aASSIGNaplusb))
    )
])
