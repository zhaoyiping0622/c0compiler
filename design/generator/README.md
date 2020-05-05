## 代码生成

### 流程

输入: AST

输出: MIPS汇编代码

处理过程: 

1. 遍历一遍AST，对每个表达式生成初步的汇编代码，此时寄存器全使用虚拟寄存器，不考虑寄存器恢复等操作(等于使用一个全局寄存器数组，返回地址操作除外)，传参、函数的栈帧需要做好，需要计算每个中间变量的生命周期，以为寄存器映射做准备，对表达式做初步的重组(存疑)。
2. 根据第一步的信息，将虚拟寄存器映射到真实寄存器，此时需要寄存器恢复操作，生成汇编代码。

输入: AST
输出: 四元组

处理过程:

1. 替换所有const
2. 常量合并
3. 对所有中间计算用中间变量表示
4. 删除冗余
5. 其他优化，待补充
6. 考虑变量的内存

其他说明: 

1. 所有中间变量都为int型
2. 对于write函数，如果要输出char，那应该将表达式赋值为一个char然后再输出

输入: 四元组
输出: MIPS汇编代码

处理过程: 

1. 寄存器分配
2. 在栈中增加中间变量的地址

### 优化算法

#### LVN

cooper P309

局部值编号算法。

#### SVN

cooper P321

超局部值编号算法

按照我现在的AST可能要改写有点麻烦