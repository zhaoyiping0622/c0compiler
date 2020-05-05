def makeArray(fun):
    def decorated(self, *args, **kwargs):
        if "root" in kwargs and kwargs["root"]:
            next = self.next  # type: AST
            self.dic = [self.toJSON()]
            while next is not None:
                self.dic.append(next.toJSON())
                next = next.next
            return self.dic
        else:
            self.dic = {}
            return fun(self, *args, **kwargs)

    return decorated

def returnSelf(fun):
    def decorated(self,*args,**kwargs):
        ret= fun(self, *args, **kwargs)
        if ret:
            return ret
        else:
            return self
    return decorated


def toJSON(x, root=False):
    if x is None:
        return None
    if type(x) is str:
        return x
    return x.toJSON(root=root)


class AST:

    def __init__(self, next=None, name=None):
        self.next = next
        self.dic = {}
        self.name = name

    def copy(self, next=None):
        from copy import deepcopy
        ret = deepcopy(self)
        a = ret
        while a.next is not None:
            a = a.next
        a.next = next
        return ret

    @returnSelf
    def setnext(self, next):
        AST.__init__(self, next=next)

    def printJSON(self):
        from json import dumps
        print(dumps(toJSON(self)))

    @makeArray
    def toJSON(self, root=False):
        self.dic["name"] = self.name
        return self.dic


class ASTDeclare(AST):

    def __init__(self, next=None, valueId=None, name=None):
        AST.__init__(self, next=next, name=name)
        self.valueId = valueId

    @returnSelf
    def setvalueId(self, valueId): self.valueId = valueId

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["valueId"] = self.valueId
        return self.dic


class ASTDeclareFun(ASTDeclare):

    def __init__(self, next=None, valueId=None, args=None, body=None, returnType=None):
        ASTDeclare.__init__(self, next=next, valueId=valueId, name="function declare")
        self.args = args
        self.body = body
        self.returnType = returnType

    @returnSelf
    def setreturnType(self, returnType):
        self.returnType = returnType

    @returnSelf
    def setargs(self, args):
        self.args = args

    @returnSelf
    def setbody(self, body):
        self.body = body

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        if self.args:
            self.dic["args"] = [{"valueType": x[0], "valueId": x[1]} for x in self.args]
        else:
            self.dic["args"] = None
        self.dic["returnType"] = self.returnType
        self.dic["body"] = toJSON(self.body, True)
        return self.dic


class ASTDeclareValue(ASTDeclare):

    def __init__(self, next=None, valueId=None, isConst=False, valueType=None, value=""):
        ASTDeclare.__init__(self, next=next, valueId=valueId, name="value declare")
        self.isConst = isConst
        self.valueType = valueType
        self.value = value

    @returnSelf
    def setisConst(self, isConst): self.isConst = isConst

    @returnSelf
    def setvalueType(self, valueType): self.valueType = valueType

    @returnSelf
    def setvalue(self, value): self.value = value

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["const"] = self.isConst
        self.dic["valueType"] = self.valueType
        self.dic["value"] = self.value
        return self.dic


class ASTDeclareArray(ASTDeclare):

    def __init__(self, next=None, valueId=None, length=8, valueType=None):
        ASTDeclare.__init__(self, next=next, valueId=valueId, name="array declare")
        self.length = length
        self.valueType = valueType

    @returnSelf
    def setlength(self, length): self.length = length

    @returnSelf
    def setvalueType(self, valueType): self.valueType = valueType

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["length"] = self.length
        self.dic["valueType"] = self.valueType
        return self.dic


class ASTCondition(AST):

    def __init__(self, next=None, cmp=None, thenStatements=None, elseStatements=None):
        AST.__init__(self, next=next, name="condition")
        self.cmp = cmp
        self.thenStatements = thenStatements
        self.elseStatements = elseStatements

    @returnSelf
    def setcmp(self, cmp): self.cmp = cmp

    @returnSelf
    def setthenStatements(self, thenStatements): self.thenStatements = thenStatements

    @returnSelf
    def setelseStatements(self, elseStatements): self.elseStatements = elseStatements

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["cmp"] = toJSON(self.cmp)
        self.dic["then"] = toJSON(self.thenStatements, root=True)
        self.dic["else"] = toJSON(self.elseStatements, root=True)
        return self.dic


class ASTLoop(AST):

    def __init__(self, next=None, cmp=None, body=None):
        AST.__init__(self, next=next, name="loop")
        self.cmp = cmp
        self.body = body

    @returnSelf
    def setcmp(self, cmp): self.cmp = cmp

    @returnSelf
    def setbody(self, body): self.body = body

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["cmp"] = toJSON(self.cmp)
        self.dic["body"] = toJSON(self.body, root=True)
        return self.dic


class ASTCall(AST):

    def __init__(self, next=None, funName=None, args=None):
        AST.__init__(self, next=next, name="call")
        self.funName = funName
        self.args = args

    @returnSelf
    def setfunName(self, funName): self.funName = funName

    @returnSelf
    def setargs(self, args): self.args = args

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["funName"] = toJSON(self.funName)
        self.dic["args"] = toJSON(self.args, root=True)
        return self.dic


class ASTExpression(AST):

    def __init__(self, next=None, operatorType=None, expression1=None, expression2=None):
        AST.__init__(self, next=next, name="expression")
        self.operatorType = operatorType
        self.expression1 = expression1
        self.expression2 = expression2

    @returnSelf
    def setoperatorType(self, operatorType): self.operatorType = operatorType

    @returnSelf
    def setexpression1(self, expression1): self.expression1 = expression1

    @returnSelf
    def setexpression2(self, expression2): self.expression2 = expression2

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["operatorType"] = self.operatorType
        self.dic["expression1"] = toJSON(self.expression1)
        self.dic["expression2"] = toJSON(self.expression2)
        return self.dic


class ASTLeaf(AST):

    def __init__(self, next=None, value=None, valueType=None):
        AST.__init__(self, next=next, name="leaf")
        self.value = value
        self.valueType = valueType

    @returnSelf
    def setvalue(self, value): self.value = value

    @returnSelf
    def setvalueType(self, valueType): self.valueType = valueType

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["value"] = self.value
        self.dic["valueType"] = self.valueType
        return self.dic


class ASTRead(AST):

    def __init__(self, next=None, args=None):
        AST.__init__(self, next=next, name="read")
        self.args = args

    @returnSelf
    def setargs(self, args): self.args = args

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["args"] = toJSON(self.args, True)
        return self.dic


class ASTWrite(AST):

    def __init__(self, next=None, args=None):
        AST.__init__(self, next=next, name="write")
        self.args = args

    @returnSelf
    def setargs(self, args): self.args = args

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["args"] = toJSON(self.args, True)
        return self.dic


class ASTRet(AST):

    def __init__(self, next=None, value=None):
        AST.__init__(self, next=next, name="ret")
        self.value = value

    @returnSelf
    def setvalue(self, value): self.value = value

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["value"] = toJSON(self.value)
        return self.dic


class ASTSwitch(AST):

    def __init__(self, next=None, expression=None, cases=None):
        AST.__init__(self, next=next, name="switch")
        self.expression = expression
        self.cases = cases

    @makeArray
    def toJSON(self, root=False):
        super().toJSON()
        self.dic["expression"] = toJSON(self.expression)
        self.dic["cases"] = toJSON(self.cases, True)
        return self.dic


a = ASTLeaf(value="a", valueType="ID")
b = ASTLeaf(value="b", valueType="ID")
c = ASTLeaf(value="c", valueType="ID")
d = ASTLeaf(value="d", valueType="ID")
num1 = ASTLeaf(value="1", valueType="UNSIGNED")
num0 = ASTLeaf(value="0", valueType="UNSIGNED")
numf1 = ASTExpression(operatorType="MINUS", expression1=num1)
char1 = ASTLeaf(value="'1'", valueType="CHARACTER")
char0 = ASTLeaf(value="'0'", valueType="CHARACTER")
charf1 = ASTExpression(operatorType="MINUS", expression1=char1)
string1 = ASTLeaf(value='"1"', valueType="STRING")
aplusb = ASTExpression(operatorType="ADD", expression1=a, expression2=b)
cplusd = ASTExpression(operatorType="ADD", expression1=c, expression2=d)
aminusb = ASTExpression(operatorType="MINUS", expression1=a, expression2=b)
amulb = ASTExpression(operatorType="MUL", expression1=a, expression2=b)
oneaddone = ASTExpression(operatorType="ADD", expression1=num1, expression2=num1)
Aarray1 = ASTExpression(operatorType="ARRAY", expression1=a, expression2=num1)
Aarrayaplusb = ASTExpression(operatorType="ARRAY", expression1=a, expression2=aplusb)
bmulc = ASTExpression(operatorType="MUL", expression1=b, expression2=c)
Aarrayb = ASTExpression(operatorType="ARRAY", expression1=a, expression2=b)
aNE0 = ASTExpression(operatorType="NE", expression1=a, expression2=num0)
aEQ0 = ASTExpression(operatorType="EQ", expression1=a, expression2=num0)
bNE0 = ASTExpression(operatorType="NE", expression1=b, expression2=num0)
bEQ0 = ASTExpression(operatorType="EQ", expression1=b, expression2=num0)
aLTb = ASTExpression(operatorType="LT", expression1=a, expression2=b)
aGTb = ASTExpression(operatorType="GT", expression1=a, expression2=b)
aLEb = ASTExpression(operatorType="LE", expression1=a, expression2=b)
aGEb = ASTExpression(operatorType="GE", expression1=a, expression2=b)
aNEb = ASTExpression(operatorType="NE", expression1=a, expression2=b)
aEQb = ASTExpression(operatorType="EQ", expression1=a, expression2=b)
aASSIGNaplusb = ASTExpression(operatorType="ASSIGN", expression1=a, expression2=aplusb)
bASSIGNaplusb = ASTExpression(operatorType="ASSIGN", expression1=b, expression2=aplusb)
UNDEFINED=ASTLeaf(value="0",valueType="UNDEFINED")
cLTd = ASTExpression(operatorType="LT", expression1=c, expression2=d)
cGTd = ASTExpression(operatorType="GT", expression1=c, expression2=d)


def joinLines(lines: list):
    for i in range(len(lines)):
        if i != 0:
            a = lines[i - 1]
            while a.next is not None:
                a = a.next
            a.next = lines[i]
    return lines


def printLines(lines: list):
    from json import dumps
    lines = joinLines(lines)
    print(dumps(lines[0].toJSON(root=True)).replace("None", "null"))


if __name__ == "__main__":
    pass
