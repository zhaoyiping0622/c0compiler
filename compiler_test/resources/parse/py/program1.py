if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(ASTDeclareValue(isConst=True, valueId="a", value="1", valueType="INT"))

lines.append(ASTDeclareValue(valueId="a", valueType="INT"))

lines.append(ASTDeclareFun(valueId="a", returnType="VOID"))
lines.append(ASTDeclareFun(valueId="a", returnType="INT"))
lines.append(ASTDeclareFun(valueId="b", returnType="VOID"))
lines.append(ASTDeclareFun(valueId="main", returnType="VOID", body=lines[0].copy(lines[1].copy())))

printLines(lines)
