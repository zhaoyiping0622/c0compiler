if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

labels = [getLabel() for i in range(100)]
addresses = [getAddress() for i in range(100)]

printTAClist([
    label("funb"),
    declareInt(localName("a")),
    declareInt(localName("b")),
    add(localName("a"), localName("b"), addresses[0]),
    setRet(addresses[0]),
    j(labels[0]),
    label(labels[0]),
    ret(),

    label("funa"),
    declareIntArg("1",localName("a")),
    declareCharArg("2",localName("b", "char")),
    sub(localName("a"), localName("b", "char"), addresses[1]),
    setRet(addresses[1]),
    j(labels[1]),
    label(labels[1]),
    ret(),

    label("main"),
    declareInt(localName("a")),
    declareInt(localName("b")),

    call("funb"),
    getRet(addresses[2]),

    add(localName("a"), localName("b"), addresses[3]),
    sub(localName("a"), localName("b"), addresses[4]),
    setArg(2, addresses[4]),
    setArg(1, addresses[3]),
    call("funa"),
    getRet(addresses[5]),

    label(labels[2]),
    ret(),
])
