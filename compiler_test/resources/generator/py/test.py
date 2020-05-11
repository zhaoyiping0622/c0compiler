if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

printTAClist([
    # declareInt(globalName("a")),
    # declareChar(globalName("b", "char")),
    # label("main"),
    # readint(globalName("a")),
    # readchar(globalName("b", "char")),
    # writeint(globalName("a")),
    # writechar(globalName("b", "char")),
    # add(globalName("a"), globalName("b", "char"), globalName("a")),
    # declareString("\"a+b=\""),
    # writestring("\"a+b=\""),
    # writeint(globalName("a")),
    # label("label1"),
    # ret(),
    label("main"),
    declareString("\"hello world!\""),
    writestring("\"hello world!\""),
    label("label1"),
    ret()
])
