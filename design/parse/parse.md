## 语法分析

### 综述

此目录下的grammar.parse文件是文法，为了便于处理，在开头添加了一列。下面是第一列标识符的解释：

+ T表示terminate，即此行为终结符
+ D表示delete，此行删除，不予考虑
+ N表示nonterminate，即此行为非终结符

设置这几个标识符是为了在不删除文件内容的情况下标识每一个符号，方便python脚本处理。

### 添加语法支持

+ character2tokentype中添加

### AST类

+ AST
+ ASTDeclare: AST
+ ASTDeclareFun: ASTDeclare
+ ASTDeclareValue: ASTDeclare
+ ASTDeclareArray: ASTDeclare
+ ASTCondition: AST
+ ASTLoop: AST
+ ASTCall: AST
+ ASTExpression: AST
+ ASTLeaf: AST
+ ASTRead: AST
+ ASTRead: AST
+ ASTWrite: AST
+ ASTRet: AST
+ ASTSwitch: AST

#### AST基类

仅含有一个非静态类成员next，以实现链表

静态类成员有`symbolTables`存放当前符号表

#### ASTDeclare

继承于AST

含有类成员`valueId`表示标识符

#### ASTDeclareFun

继承于ASTDeclare

用于函数声明

类成员
+ `returnType` 表示返回类型
+ `args` 参数列表 按照 pair<类型, 标识符> 形成一个vector
+ `body` 函数体，是一个由若干抽象语法树组成的链表

#### ASTDeclareValue

继承于ASTDeclare

用于变量声明

类成员
+ `isConst` 表示变量是否可变
+ `valueType` 变量类型
+ `value` 变量的值 根据文法，只有在变量不可变时这个值为非0，否则为0

#### ASTDeclareArray

继承于ASTDeclare

用于数组声明

类成员
+ `length` 数组长度
+ `valueType` 数组类型

#### ASTCondition

继承于AST

用于if语句

类成员

+ `cmp` 比较表达式
+ `thenStatements` 比较为真时执行的语句
+ `elseStatements` 比较为假时执行的语句

#### ASTLoop

继承于AST

用于循环语句

类成员

+ `cmp` 比较表达式
+ `body` 比较为真时执行的语句

#### ASTCall

继承于AST

用于函数调用

类成员

+ `funName` 调用函数名
+ `args` 参数列表，这是由AST组成的链表

#### ASTExpression

继承于AST

用于表达式

类成员 
+ `operatorType` 运算符，可以是 `+ - * / && || > < != >= <= ! == = []`
+ `expression1` 左表达式
+ `expression2` 右表达式，如果是单目运算符则为空指针NULL或UNDEFINED

#### ASTLeaf

继承于AST

叶子节点

类成员
+ `value` 值
+ `valueType` 叶子节点类型 可以是 ID character integer unsigned 

#### ASTRead

继承于AST

输入函数

类成员
+ `args` 输入的值列表

#### ASTWrite

继承于AST

输出函数

类成员
+ `args` 输出的值列表

#### ASTRet

继承于AST

返回

类成员
+ `value` 返回的值

#### ASTSwitch

继承于AST

switch

类成员
+ `expression` 表达式
+ `cases` case以及执行语句 按照 default -> ASTLeaf -> AST -> ASTLeaf -> AST 排列
