if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    label("main"),
    declareInt(localName("a")),
    declareChar(localName("b", "char")),
    add(localName("a"), localName("b", "char"), getAddress()),
    add(localName("b", "char"), "1", getAddress()),
    add(localName("a"), "49", getAddress()),
    label(getLabel()),
    ret()
])
