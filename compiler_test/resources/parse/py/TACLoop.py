if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

labels = [getLabel() for i in range(100)]
addresses = [getAddress() for i in range(100)]

printTAClist([
    label("main"),
    declareInt(localName("a")),
    declareInt(localName("b")),

    label(labels[1]),  # begin
    jl(localName("a"), localName("b"), labels[2]),
    j(labels[3]),
    label(labels[2]),
    add(localName("a"), localName("b"), addresses[0]),
    j(labels[1]),
    label(labels[3]),

    label(labels[0]),
    ret()
])
