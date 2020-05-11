if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    label("funa"),
    declareIntArg(localName("a")),
    declareCharArg(localName("b", "char")),
    label(getLabel()),
    ret(),
    label("funb"),
    declareIntArg(localName("a")),
    label(getLabel()),
    ret(),
    label("main"),
    label(getLabel()),
    ret()
])
