if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    declareInt(globalName("c")),
    declareChar(globalName("d", "char")),
    label("main"),
    declareInt(localName("c")),
    declareChar(localName("d", "char")),
    label(getLabel()),
    ret()
])
