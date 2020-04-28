if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

lines=[
    ASTDeclareFun(
        valueId="funa",
        body=ASTDeclareFun(
            valueId="funb",
            returnType="VOID"
        ),
        returnType="VOID"
    )
]
printLines(lines)