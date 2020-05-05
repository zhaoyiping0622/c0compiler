def localName(name, valueType="int"):
    if valueType.lower() == "int":
        return "li" + name
    elif valueType.lower() == "char":
        return "lc" + name


def globalName(name, valueType="int"):
    if valueType.lower() == "int":
        return "gi" + name
    elif valueType.lower() == "char":
        return "gc" + name


class TAC:
    def __init__(self, op="TACUNDEFINED", ad1="", ad2="", ad3=""):
        self.op = op
        self.ad1 = ad1
        self.ad2 = ad2
        self.ad3 = ad3

    def toDict(self):
        return {
            "op": str(self.op),
            "ad1": str(self.ad1),
            "ad2": str(self.ad2),
            "ad3": str(self.ad3),
        }


def add(ad1, ad2, ad3) -> TAC:
    return TAC("TACADD", ad1, ad2, ad3)


def sub(ad1, ad2, ad3) -> TAC:
    return TAC("TACSUB", ad1, ad2, ad3)


def mul(ad1, ad2, ad3) -> TAC:
    return TAC("TACMUL", ad1, ad2, ad3)


def div(ad1, ad2, ad3) -> TAC:
    return TAC("TACDIV", ad1, ad2, ad3)


def mov(ad1, ad3) -> TAC:
    return TAC("TACMOV", ad1, "", ad3)


def getArr(ad1, ad2, ad3) -> TAC:
    return TAC("TACGETARR", ad1, ad2, ad3)


def setArr(ad1, ad2, ad3) -> TAC:
    return TAC("TACSETARR", ad1, ad2, ad3)


def jle(ad1, ad2, ad3) -> TAC:
    return TAC("TACJLE", ad1, ad2, ad3)


def jge(ad1, ad2, ad3) -> TAC:
    return TAC("TACJGE", ad1, ad2, ad3)


def jl(ad1, ad2, ad3) -> TAC:
    return TAC("TACJL", ad1, ad2, ad3)


def jg(ad1, ad2, ad3) -> TAC:
    return TAC("TACJG", ad1, ad2, ad3)


def jne(ad1, ad2, ad3) -> TAC:
    return TAC("TACJNE", ad1, ad2, ad3)


def jeq(ad1, ad2, ad3) -> TAC:
    return TAC("TACJEQ", ad1, ad2, ad3)


def call(ad3) -> TAC:
    return TAC("TACCALL", "", "", ad3)


def ret() -> TAC:
    return TAC("TACRET", "", "", "")


def getRet(ad3) -> TAC:
    return TAC("TACGETRET", "", "", ad3)


def setRet(ad3) -> TAC:
    return TAC("TACSETRET", "", "", ad3)


def setArg(ad1, ad3) -> TAC:
    return TAC("TACSETARG", ad1, "", ad3)


def getArg(ad1, ad3) -> TAC:
    return TAC("TACGETARG", ad1, "", ad3)


def j(ad3) -> TAC:
    return TAC("TACJ", "", "", ad3)


def readint(ad3) -> TAC:
    return TAC("TACREADINT", "", "", ad3)


def readchar(ad3) -> TAC:
    return TAC("TACREADCHAR", "", "", ad3)


def writeint(ad3) -> TAC:
    return TAC("TACWRITEINT", "", "", ad3)


def writechar(ad3) -> TAC:
    return TAC("TACWRITECHAR", "", "", ad3)


def writestring(ad3) -> TAC:
    return TAC("TACWRITESTRING", "", "", ad3)


def declareChar(ad3) -> TAC:
    return TAC("TACDECLARECHAR", "", "", ad3)


def declareInt(ad3) -> TAC:
    return TAC("TACDECLAREINT", "", "", ad3)


def declareString(ad3) -> TAC:
    return TAC("TACDECLARESTRING", "", "", ad3)


def declareArrayInt(ad1, ad3) -> TAC:
    return TAC("TACDECLAREARRAYINT", ad1, "", ad3)


def declareArrayChar(ad1, ad3) -> TAC:
    return TAC("TACDECLAREARRAYCHAR", ad1, "", ad3)


def label(ad3) -> TAC:
    return TAC("TACLABEL", "", "", ad3)


def TAClist2JSON(TAClist: list):
    from json import dumps
    return dumps([x.toDict() for x in TAClist])


def printTAClist(TAClist: list):
    print(TAClist2JSON(TAClist).replace("None", "null"))


class newAddress:
    def __init__(self):
        self.cnt = 0

    def __call__(self, *args, **kwargs):
        self.cnt += 1
        if len(args)==0:
            return "ti" + str(self.cnt)
        if str(args[0]).lower() == "int":
            return "ti" + str(self.cnt)
        elif str(args[0]).lower() == "char":
            return "tc" + str(self.cnt)

class newLabel:
    def __init__(self):
        self.cnt = 0

    def __call__(self, *args, **kwargs):
        self.cnt += 1
        return "label"+str(self.cnt)


getAddress = newAddress()
getLabel = newLabel()
