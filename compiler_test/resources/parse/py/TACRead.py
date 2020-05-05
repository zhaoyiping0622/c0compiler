if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    declareInt(globalName("a")),
    label("main"),
    declareInt(localName("b")),
    declareChar(localName("c", "char")),
    readint(globalName("a")),
    readint(localName("b")),
    readchar(localName("c", "char")),
    label(getLabel()),
    ret()
])
