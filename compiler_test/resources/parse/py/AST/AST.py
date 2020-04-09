def toJSON(x):
    if x is None:
        return None
    return x.toJSON()


class AST:

    def __init__(self, next=None, name=None):
        self.next = next
        self.dic = {}
        self.name = name

    def setnext(self, next):
        AST.__init__(self, next=next)

    def printJSON(self):
        from json import dumps
        print(dumps(toJSON(self)))

    def toJSON(self):
        if self.next:
            self.dic["next"] = toJSON(self.next)
        else:
            self.dic["next"] = None
        self.dic["name"] = self.name
        return self.dic


class ASTDeclare(AST):

    def __init__(self, next=None, valueId=None, name=None):
        AST.__init__(self, next=next, name=name)
        self.valueId = valueId

    def setvalueId(self, valueId): self.valueId = valueId

    def toJSON(self):
        super().toJSON()
        self.dic["valueId"] = self.valueId
        return self.dic


class ASTDeclareFun(ASTDeclare):

    def __init__(self, next=None, valueId=None, args=None, body=None, returnType=None):
        ASTDeclare.__init__(self, next=next, valueId=valueId, name="function declare")
        self.args = args
        self.body = body
        self.returnType = returnType

    def setreturnType(self, returnType): self.returnType = returnType

    def setargs(self, args): self.args = args

    def setbody(self, body): self.body = body

    def toJSON(self):
        super().toJSON()
        self.dic["args"] = [{"valueType": x[0], "valueId": x[1]} for x in self.args]
        self.dic["returnType"] = self.returnType
        self.dic["body"] = toJSON(self.body)
        return self.dic


class ASTDeclareValue(ASTDeclare):

    def __init__(self, next=None, valueId=None, isConst=False, valueType=None, value=None):
        ASTDeclare.__init__(self, next=next, valueId=valueId, name="value declare")
        self.isConst = isConst
        self.valueType = valueType
        self.value = value

    def setisConst(self, isConst): self.isConst = isConst

    def setvalueType(self, valueType): self.valueType = valueType

    def setvalue(self, value): self.value = value

    def toJSON(self):
        super().toJSON()
        self.dic["const"] = self.isConst
        self.dic["valueType"] = self.valueType
        self.dic["value"] = self.value
        return self.dic


class ASTDeclareArray(ASTDeclare):

    def __init__(self, next=None, valueId=None, length=None, valueType=None):
        ASTDeclare.__init__(self, next=next, valueId=valueId, name="array declare")
        self.length = length
        self.valueType = valueType

    def setlength(self, length): self.length = length

    def setvalueType(self, valueType): self.valueType = valueType

    def toJSON(self):
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

    def setcmp(self, cmp): self.cmp = cmp

    def setthenStatements(self, thenStatements): self.thenStatements = thenStatements

    def setelseStatements(self, elseStatements): self.elseStatements = elseStatements

    def toJSON(self):
        super().toJSON()
        self.dic["cmp"] = toJSON(self.cmp)
        self.dic["then"] = toJSON(self.thenStatements)
        self.dic["else"] = toJSON(self.elseStatements)
        return self.dic


class ASTLoop(AST):

    def __init__(self, next=None, cmp=None, body=None):
        AST.__init__(self, next=next, name="loop")
        self.cmp = cmp
        self.body = body

    def setcmp(self, cmp): self.cmp = cmp

    def setbody(self, body): self.body = body

    def toJSON(self):
        super().toJSON()
        self.dic["cmp"] = toJSON(self.cmp)
        self.dic["body"] = toJSON(self.body)
        return self.dic


class ASTCall(AST):

    def __init__(self, next=None, funName=None, args=None):
        AST.__init__(self, next=next, name="call")
        self.funName = funName
        self.args = args

    def setfunName(self, funName): self.funName = funName

    def setargs(self, args): self.args = args

    def toJSON(self):
        super().toJSON()
        self.dic["funName"] = toJSON(self.funName)
        self.dic["args"] = toJSON(self.args)
        return self.dic


class ASTStatement(AST):

    def __init__(self, next=None, operatorType=None, statement1=None, statement2=None):
        AST.__init__(self, next=next, name="statement")
        self.operatorType = operatorType
        self.statement1 = statement1
        self.statement2 = statement2

    def setoperatorType(self, operatorType): self.operatorType = operatorType

    def setstatement1(self, statement1): self.statement1 = statement1

    def setstatement2(self, statement2): self.statement2 = statement2

    def toJSON(self):
        super().toJSON()
        self.dic["operatorType"] = self.operatorType
        self.dic["statement1"] = toJSON(self.statement1)
        self.dic["statement2"] = toJSON(self.statement2)
        return self.dic


class ASTLeaf(AST):

    def __init__(self, next=None, value=None, valueType=None):
        AST.__init__(self, next=next, name="leaf")
        self.value = value
        self.valueType = valueType

    def setvalue(self, value): self.value = value

    def setvalueType(self, valueType): self.valueType = valueType

    def toJSON(self):
        super().toJSON()
        self.dic["value"] = self.value
        self.dic["valueType"] = self.valueType
        return self.dic


class ASTRead(AST):

    def __init__(self, next=None, args=None):
        AST.__init__(self, next=next, name="read")
        self.args = args

    def setargs(self, args): self.args = args

    def toJSON(self):
        super().toJSON()
        self.dic["args"] = toJSON(self.args)
        return self.dic


class ASTWrite(AST):

    def __init__(self, next=None, args=None):
        AST.__init__(self, next=next, name="write")
        self.args = args

    def setargs(self, args): self.args = args

    def toJSON(self):
        super().toJSON()
        self.dic["args"] = toJSON(self.args)
        return self.dic


class ASTRet(AST):

    def __init__(self, next=None, value=None):
        AST.__init__(self, next=next, name="ret")
        self.value = value

    def setvalue(self, value): self.value = value

    def toJSON(self):
        super().toJSON()
        self.dic["value"] = toJSON(self.value)
        return self.dic


class ASTSwitch(AST):

    def __init__(self, next=None, expression=None, cases=None):
        AST.__init__(self, next=next, name="switch")
        self.expression = expression
        self.cases = cases

    def toJSON(self):
        super().toJSON()
        self.dic["expression"] = toJSON(self.expression)
        self.dic["cases"] = toJSON(self.cases)
        return self.dic


if __name__ == "__main__":
    pass
