from json import dumps

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

inta = ASTDeclareValue(valueId='a', valueType='INT',value="")
intc = ASTDeclareValue(valueId='c', valueType='INT',value="")
intd = ASTDeclareArray(valueId='d', length=9, valueType='INT')
inte = ASTDeclareValue(valueId='e', valueType='INT',value="")
chara = ASTDeclareValue(valueId='a', valueType='CHAR',value="")
charc = ASTDeclareValue(valueId='c', valueType='CHAR',value="")
chard = ASTDeclareArray(valueId='d', length=9, valueType='CHAR')
chare = ASTDeclareValue(valueId='e', valueType='CHAR',value="")
inta.next = intc
intc.next = intd
intd.next = inte
inte.next = chara
chara.next = charc
charc.next = chard
chard.next = chare
print(dumps(inta.toJSON()).replace('None', 'null'))
