if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    label("funa"),
    declareIntArg("1", localName("a")),
    declareCharArg("2", localName("b", "char")),
    label(getLabel()),
    ret(),
    label("funb"),
    declareIntArg("1", localName("a")),
    label(getLabel()),
    ret(),
    label("main"),
    label(getLabel()),
    ret()
])
