if __name__ == "__main__":
    from TAC.TAC import *
else:
    from .TAC.TAC import *

labels = [getLabel() for i in range(100)]

printTAClist([
    label("main"),
    declareInt(localName("a")),
    declareInt(localName("b")),
    declareArrayInt("8", localName("c")),
    add(localName("a"), localName("b"), getAddress()),
    sub(localName("a"), localName("b"), getAddress()),
    mul(localName("a"), localName("b"), getAddress()),
    div(localName("a"), localName("b"), getAddress()),
    sub("0", localName("a"), getAddress()),
    # 1 2 1 3 2 3
    jl(localName("a"), localName("b"), labels[1]),
    j(labels[2]),
    label(labels[1]),
    add(localName("a"), localName("b"), getAddress()),
    j(labels[3]),
    label(labels[2]),
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[3]),
    # 4 5 4 6 5 6
    jle(localName("a"), localName("b"), labels[4]),
    j(labels[5]),
    label(labels[4]),
    add(localName("a"), localName("b"), getAddress()),
    j(labels[6]),
    label(labels[5]),
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[6]),
    # 7 8 7 9 8 9
    jg(localName("a"), localName("b"), labels[7]),
    j(labels[8]),
    label(labels[7]),
    add(localName("a"), localName("b"), getAddress()),
    j(labels[9]),
    label(labels[8]),
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[9]),
    # 10 11 10 12 11 12
    jge(localName("a"), localName("b"), labels[10]),
    j(labels[11]),
    label(labels[10]),
    add(localName("a"), localName("b"), getAddress()),
    j(labels[12]),
    label(labels[11]),
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[12]),
    # 13 14 13 15 14 15
    jne(localName("a"), localName("b"), labels[13]),
    j(labels[14]),
    label(labels[13]),
    add(localName("a"), localName("b"), getAddress()),
    j(labels[15]),
    label(labels[14]),
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[15]),
    # 16 17 16 18 17 18
    jeq(localName("a"), localName("b"), labels[16]),
    j(labels[17]),
    label(labels[16]),
    add(localName("a"), localName("b"), getAddress()),
    j(labels[18]),
    label(labels[17]),
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[18]),

    jl(localName("a"), localName("b"), labels[21]),
    j(labels[20]),
    label(labels[21]),
    jg(localName("a"), localName("b"), labels[19]),
    j(labels[20]),
    label(labels[19]),  # then
    add(localName("a"), localName("b"), getAddress()),
    j(labels[22]),
    label(labels[20]),  # else
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[22]),

    jl(localName("a"), localName("b"), labels[23]),
    j(labels[25]),
    label(labels[25]),
    jg(localName("a"), localName("b"), labels[23]),
    j(labels[24]),
    label(labels[23]),  # then
    add(localName("a"), localName("b"), getAddress()),
    j(labels[26]),
    label(labels[24]),  # else
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[26]),

    jl(localName("a"), localName("b"), labels[28]),
    j(labels[27]),
    label(labels[27]),  # then
    add(localName("a"), localName("b"), getAddress()),
    j(labels[29]),
    label(labels[28]),  # else
    sub(localName("a"), localName("b"), getAddress()),
    label(labels[29]),

    add(localName("a"), localName("b"), tmp := getAddress()),
    mov(tmp, localName("a")),

    getArr(localName("c"), localName("a"), tmp := getAddress()),
    add(tmp, localName("b"), getAddress()),

    add(localName("a"), "0", (getAddress(),getAddress())[0]),

    setArr(localName("c"), localName("a"), localName("b")),

    label(labels[0]),
    ret()
])
