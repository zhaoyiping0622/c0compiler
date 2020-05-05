if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    label("main"),
    declareInt(localName("a")),
    declareChar(localName("b","char")),
    writeint(localName("a")),
    writechar(localName("b", "char")),
    declareString("\"a\""),
    writestring("\"a\""),
    label(getLabel()),
    ret()
])
