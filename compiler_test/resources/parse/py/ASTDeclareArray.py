if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    arrayInta.setlength(1),
    arrayCharb.setlength(2),
    main.setbody(
        arrayIntc.setlength(3).copy(arrayChard.setlength(4))
    )
])
