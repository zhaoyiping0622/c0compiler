if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

a = ASTDeclareValue(valueId="a", isConst=True, valueType="INT", value="1")
b = ASTDeclareValue(valueId="b", isConst=True, valueType="CHAR", value="'1'")
d = ASTDeclareValue(valueId="d", isConst=True, valueType="INT", value="1")
e = ASTDeclareValue(valueId="e", isConst=True, valueType="CHAR", value="'1'")
c = ASTDeclareValue(valueId="c", isConst=False, valueType="INT", value="2")
C = ASTDeclareValue(valueId="C", isConst=False, valueType="CHAR", value="'2'")
f = ASTDeclareValue(valueId="f", isConst=False, valueType="INT", value="2")
array = ASTDeclareArray(valueId="g", length=8, valueType="INT")
IF = ASTCondition(cmp=aNE0)

fun1 = ASTDeclareFun(
    valueId="fun1",
    args=[("INT","c"),("CHAR","C")],
    body=a.copy(b.copy(d.copy(e.copy(f.copy(array.copy(IF.copy())))))),
    returnType="INT"
)
fun2 = ASTDeclareFun(
    valueId="fun2",
    args=[("INT","c"),("CHAR","C")],
    body=a.copy(b.copy(d.copy(e.copy(f.copy(IF.copy()))))),
    returnType="CHAR"
)
funMain = ASTDeclareFun(
    valueId="main",
    body=a.copy(b.copy(d.copy(e.copy(f.copy(IF.copy()))))),
    returnType="VOID"
)

lines = [
    a.copy(),
    b.copy(),
    d.copy(),
    e.copy(),
    c.copy(),
    fun1,
    fun2,
    funMain
]
printLines(lines)
