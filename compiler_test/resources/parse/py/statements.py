if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []

lines.append(
    ASTCondition(
        cmp=aNE0,
        thenStatements=aASSIGNaplusb,
        elseStatements=bASSIGNaplusb
    )
)
lines.append(
    ASTLoop(
        cmp=aNE0,
        body=aASSIGNaplusb
    )
)
lines.append(lines[0].copy(lines[1].copy()))
lines.append(ASTCall(funName="a"))
lines.append(aASSIGNaplusb.copy())
lines.append(ASTRead(args=a.copy(b.copy(c))))
lines.append(ASTWrite(args=string1.copy(char1.copy(num1.copy(c)))))
lines.append(ASTExpression(operatorType="UNDEFINED"))
lines.append(ASTSwitch(expression=a))
lines.append(ASTRet(value=a))

printLines(lines)
