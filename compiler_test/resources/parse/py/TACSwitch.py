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
    sub(localName("a"), localName("b"), addresses[0]),
    add(localName("a"), localName("b"), addresses[1]),

    sub(localName("a"), localName("b"), addresses[2]),
    jeq("49", addresses[2], labels[2]),
    j(labels[1]),

    label(labels[2]),
    add(localName("a"), localName("b"), addresses[3]),
    j(labels[1]),

    label(labels[1]),

    # case 3

    add(localName("a"), localName("b"), addresses[4]),
    jeq("0", addresses[4], labels[4]),
    jeq("1", addresses[4], labels[5]),
    j(labels[3]),

    label(labels[4]),
    add(localName("a"), localName("b"), addresses[5]),
    j(labels[3]),
    label(labels[5]),
    sub(localName("a"), localName("b"), addresses[6]),
    j(labels[3]),

    label(labels[3]),

    # case 4

    add(localName("a"), localName("b"), addresses[7]),
    jeq("0", addresses[7], labels[8]),
    jeq("1", addresses[7], labels[9]),
    j(labels[7]),
    # default
    label(labels[7]),
    sub(localName("a"), localName("b"), addresses[8]),
    j(labels[6]),
    # cases
    label(labels[8]),
    add(localName("a"), localName("b"), addresses[9]),
    j(labels[6]),
    label(labels[9]),
    sub(localName("a"), localName("b"), addresses[10]),
    j(labels[6]),
    label(labels[6]),

    # case 5

    # 0-8
    jeq("4", localName("a"), labels[15]),  # 4
    jg("4", localName("a"), labels[20]),
    # 0-3
    jeq("1", localName("a"), labels[12]),  # 1
    jg("1", localName("a"), labels[21]),
    # 0-0
    jeq("0", localName("a"), labels[11]),  # 0
    j(labels[10]),
    label(labels[21]),  # g 1
    # 2-3
    jeq("2", localName("a"), labels[13]),  # 2
    jg("2", localName("a"), labels[22]),
    # 2-1
    j(labels[10]),
    label(labels[22]),  # g 2
    # 3-3
    jeq("3", localName("a"), labels[14]),  # 3
    j(labels[10]),
    label(labels[20]),  # g 4
    # 5-8
    jeq("6", localName("a"), labels[17]),  # 6
    jg("6", localName("a"), labels[23]),
    # 5-5
    jeq("5", localName("a"), labels[16]),  # 5
    j(labels[10]),
    label(labels[23]),  # g 6
    # 7-8
    jeq("7", localName("a"), labels[18]),  # 7
    jg("7", localName("a"), labels[24]),
    # 7-6
    j(labels[10]),
    label(labels[24]),  # g 7
    # 8-8
    jeq("8", localName("a"), labels[19]), # 8
    j(labels[10]),

    label(labels[11]),
    add(localName("a"), localName("b"), addresses[11]),
    j(labels[10]),
    label(labels[12]),
    add(localName("a"), localName("b"), addresses[12]),
    j(labels[10]),
    label(labels[13]),
    add(localName("a"), localName("b"), addresses[13]),
    j(labels[10]),
    label(labels[14]),
    add(localName("a"), localName("b"), addresses[14]),
    j(labels[10]),
    label(labels[15]),
    add(localName("a"), localName("b"), addresses[15]),
    j(labels[10]),
    label(labels[16]),
    add(localName("a"), localName("b"), addresses[16]),
    j(labels[10]),
    label(labels[17]),
    add(localName("a"), localName("b"), addresses[17]),
    j(labels[10]),
    label(labels[18]),
    add(localName("a"), localName("b"), addresses[18]),
    j(labels[10]),
    label(labels[19]),
    add(localName("a"), localName("b"), addresses[19]),
    j(labels[10]),

    # switch end
    label(labels[10]),

    label(labels[0]),
    ret()
])
