if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

lines = []
lines.append(ASTRead(args=a))
lines.append(ASTRead(args=a.copy(b.copy(c))))

printLines(lines)
