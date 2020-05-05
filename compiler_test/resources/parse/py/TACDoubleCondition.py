if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

labels = [getLabel() for i in range(100)]
addresses = [getAddress() for i in range(100)]

printTAClist([
    label("main"),
    declareInt(localName('a')),
    declareInt(localName('b')),
    jl(localName('a'), localName('b'), labels[1]),
    j(labels[2]),
    label(labels[1]),  # then1
    jl(localName('a'), localName('b'), labels[3]),
    j(labels[4]),
    label(labels[3]),  # then2
    add(localName('a'), localName('b'), addresses[0]),
    label(labels[4]),  # end2
    j(labels[5]),
    label(labels[2]),  # else1
    jl(localName('a'), localName('b'), labels[6]),
    j(labels[7]),
    label(labels[6]),  # then3
    add(localName('a'), localName('b'), addresses[1]),
    j(labels[8]),
    label(labels[7]),  # else3
    sub(localName('a'), localName('b'), addresses[2]),
    label(labels[8]),  # end3
    label(labels[5]),
    label(labels[0]),
    ret()
])
