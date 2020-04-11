if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []

lines.append(ASTDeclareFun(valueId="a", returnType="INT"))
lines.append(ASTDeclareFun(valueId="b", returnType="CHAR", args=[("INT", "a")]))
lines.append(ASTDeclareFun(valueId="a", returnType="VOID", args=[("INT", "b"), ("CHAR", "c"), ("INT", "a")],
                           body=ASTDeclareValue(isConst=True, valueId="a", value="1", valueType="INT").copy(
                               ASTDeclareValue(valueId="b", valueType="INT"))))

printLines(lines)
