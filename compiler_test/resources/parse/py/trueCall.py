if __name__ == "__main__":
    from AST.AST import *
    from AST.declares import *
else:
    from .AST.AST import *
    from .AST.declares import *

printLines([
    voidFuna,  # no arg
    intFunb.setargs([("INT", "a"), ("CHAR", "b")]),
    charFunc.setargs([("INT", "c")]),
    main.setbody(
        inta.copy(intb.copy(charc# int a;int b;char c;
            .copy(ASTCall(funName="funa")# funa();
            .copy(ASTExpression(operatorType="ASSIGN",expression1=a.copy(),expression2=ASTCall(funName="funb",args=a.copy(b.copy())))# a=funb(a,b)
            .copy(ASTExpression(operatorType="ASSIGN",expression1=c.copy(),expression2=ASTCall(funName="funb",args=a.copy(b.copy())))# c=funb(a,b)
            .copy(ASTExpression(operatorType="ASSIGN",expression1=a.copy(),expression2=ASTCall(funName="func",args=b.copy()))) # a=func(b)
            ))))))
])
