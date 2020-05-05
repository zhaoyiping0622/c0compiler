if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    declareArrayInt(1,globalName("a")),
    declareArrayChar(2,globalName("b","char")),
    label("main"),
    declareArrayInt(3,localName("c")),
    declareArrayChar(4,localName("d","char")),
    label(getLabel()),
    ret()
])
