if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

la = getLabel()  # main end
lb = getLabel()  # then
lc = getLabel()  # else
ld = getLabel()  # if end
le = getLabel()  # then
lf = getLabel()  # else

a1 = getAddress()
a2 = getAddress()
a3 = getAddress()

printTAClist([
    label("main"),
    declareInt(localName("a")),
    declareInt(localName("b")),
    jl(localName("a"), localName("b"), lb),
    j(lc),
    label(lb),
    add(localName('a'), localName('b'), a1),
    mov(a1, localName('a')),
    j(ld),
    label(lc),
    add(localName('a'), localName('b'), a2),
    mov(a2, localName('b')),
    label(ld),
    jl(localName('a'), localName('b'), le),
    j(lf),
    label(le),
    add(localName('a'), localName('b'), a3),
    mov(a3, localName('a')),
    label(lf),
    label(la),
    ret()
])
