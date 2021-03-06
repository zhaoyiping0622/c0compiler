## 词法分析

文法可参照[grammar.en](./grammar/grammar.en)

### 分析

首先是一个问题：哪些部分需要在词法分析过程中识别，哪些部分是词法分析器交给语法分析器处理。

对于这个问题，我认为以下几个部分是词法分析器提取的

+ 保留字
  + int
  + char
  + void
  + if
  + while
  + switch
  + case
  + scanf
  + printf
  + return
  + main
  + default
  + const
  + else
+ 不可分解的元素
  + addOp(+-)
  + mulOp(*/)
  + cmpOp(</<=/>/>=/!=/==)
  + boolOp(!/||/&&)
  + semicolon;
  + sbrackets()
  + bbrackets{}
  + mbrackets[]
  + assign=
  + comma,
+ 常量
  + char \\\'[+-*/_a-zA-Z0-9]\\\'
  + string \\\"[\x20\x21\x23-\x7e]\\\"
  + unsigned [0-9]*
+ 标识符

在正则表达式中，保留字和标识符一起处理，只是需要预处理将保留字放入符号表中，并标记为相应语义。

### 编码思路

此文件夹中有四个文件夹`cpp`/`dot`/`png`/`txt`，其中`txt`保存的是最原始的文件，其他文件夹中的文件都是由`txt`文件夹中的文件生成。

`txt`文件夹中的文件定义规则：

1.第一部分是终止结点声明，每行只有空格分隔的两个单词，第一个单词是一个数字，作为结点编号，将某一个结点声明为终止结点，也就是说从初始节点1沿DFA走到这个结点，可以生成一个Token，Token中的tokentype为本行第二个单词。
2.剩余部分是结点连接部分，每行分为三个部分，第一个部分是起点，第二个部分是终点，第三个部分是以空格分隔的规则，意思是满足满足规则的字符可以使得状态从起点变为终点，规则如下：
  + 单独一个字符表示与此字符相同
  + 一个0x开头的数字表示ASCII码为此数字的字符
  + A-B，A和B可以是字符或以0x开头的ASCII码（不能混搭，因为我第一次写的时候没有混搭，现在懒得改了），表示从A到B的所有字符（闭区间）
  
通过txt中的文件可生成cpp和dot文件，txt文件夹中有一个`total.py`用来组合所有txt文件。通过dot文件可生成png文件。

### 增加语法支持时需要注意的东西

自动机部分直接拷贝`design/scan/cpp`中内容至`scan.cpp`中`initTable`函数

#### 增加保留字

+ `scan.h`中`tokentype`添加保留字
+ `testscan.cpp`中`SetUp`函数中添加保留字
+ 如果是关键字还需要添加`keywords`函数

#### 增加运算符

+ `scan.h`中`tokentype`添加保留字

### 遇到的问题

#### 错误处理

最主要的问题是在标识符处理上，类似于'100asdf'这种不合法的标识符，不是很确定应该在这里直接报错还是由语法分析器报错，现在暂时的处理是在这里将其识别为'100'和'asdf'两个部分，然后由语法分析器报错。我的主要理由如下：
+ 如果在词法分析器中报错，那么需要在DFA中增加一个error结点，也就是从一个终结状态指向一个error状态，那么判断当前是转移为error状态还是结束并返回一个Token需要添加大量规则，太麻烦。
+ 在词法分析中不合法的Token，分解成两个Token在语法分析器中也会发现是非法的，并不会影响程序的正确性。

因此，词法分析器只在以下情况下报错：
+ !后面不是=，因为文法不支持非运算
+ character不合法，例如：''','','\\','abcdef'
+ string不合法，例如：""","\\",横跨多行的字符串
