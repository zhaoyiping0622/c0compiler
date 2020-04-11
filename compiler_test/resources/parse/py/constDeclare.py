from json import dumps

if __name__ == "__main__":
    from AST.AST import *
else:
    from .AST.AST import *

inta = ASTDeclareValue(isConst=True, valueId="a", valueType="INT", value="1")
intb = ASTDeclareValue(isConst=True, valueId="b", valueType="INT", value="1")
intc = ASTDeclareValue(isConst=True, valueId="c", valueType="INT", value="-2")
intd = ASTDeclareValue(isConst=True, valueId="d", valueType="INT", value="3")
chara = ASTDeclareValue(isConst=True, valueId="a", valueType="CHAR", value="'1'")
charb = ASTDeclareValue(isConst=True, valueId="b", valueType="CHAR", value="'2'")
charc = ASTDeclareValue(isConst=True, valueId="c", valueType="CHAR", value="'1'")
inta.next = intb
intb.next = intc
intc.next = intd
intd.next = chara
chara.next = charb
charb.next = charc
print(dumps(inta.toJSON()).replace("None", "null"))
