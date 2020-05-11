## 代码生成

### 流程

输入: AST
输出: 三地址码

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

输入: 三地址码
输出: MIPS汇编代码

处理过程: 

1. 寄存器分配
2. 构造栈帧

栈帧
```text
 * sp-before(bsp)->| restore registerN |
 *                 | ....              |
 *                 | arg4              |
 *                 | arg3              |
 *                 | arg2              | +($fp)
 * fp------------->| arg1 size = 4     | 0($fp)
 *                 | declare value1    | -($fp)
 *                 | declare value2    |
 *                 | declare value3    |
 *                 | ..............    |
 *                 | declare valueN    |
 *                 | tmp value1        |
 *                 | tmp value2        |
 *                 | tmp value3        |
 *                 | ..........        |
 *                 | tmp valueN        |
 *                 | bra               |
 *                 | bsp               |
 *                 | bfp               |
 *                 | restore register1 |
 *                 | restore register2 |
 *                 | restore register3 |
 *                 | ................. |
 * sp------------->| restore registerN |
```

### 寄存器分配

#### 类设计

首先寄存器分配策略是对外不可见的。当生成器需要寄存器时向寄存器分配器发起请求，寄存器分配器根据传入的地址分配寄存器，将地址或地址中的值导入寄存器的过程是对外不可见的。

寄存器分配器的功能越简单越好，应该让大部分功能由代码生成器实现。

寄存器分配器执行以下功能：

+ 寄存器分配
+ 栈帧建立(因为保存多少寄存器需要寄存器分配器决定，因此无法在代码生成器处处理栈帧)
+ data段建立(data段只处理全局变量)

### 优化算法

#### LVN

cooper P309

局部值编号算法。

#### SVN

cooper P321

超局部值编号算法

按照我现在的AST可能要改写有点麻烦
