if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    label("funa"),
    declareInt(localName("a")),
    getArg(1, localName("a")),
    declareChar(localName("b", "char")),
    getArg(2, localName("b", "char")),
    label(getLabel()),
    ret(),
    label("funb"),
    declareInt(localName("a")),
    getArg(1, localName("a")),
    label(getLabel()),
    ret(),
    label("main"),
    label(getLabel()),
    ret()
])
