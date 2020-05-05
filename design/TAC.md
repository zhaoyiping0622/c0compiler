## TAC

three address code

| operator              | ad1  | ad2  | ad3  | 备注                              |
| --------------------- | ---- | ---- | ---- | --------------------------------- |
| add/sub/mul/div       | 1    | 1    | 1    | 结果放在ad3                       |
| mov                   | 1    | 0    | 1    | 将ad1放入ad3                      |
| getArr                | 1    | 1    | 1    | ad3=ad1[ad2]                      |
| setArr                | 1    | 1    | 1    | ad1[ad2]=ad3                      |
| jle/jge/jne/jeq/jg/jl | 1    | 1    | 1    | jle: if ad1<ad2 jump ad3 其他类推 |
| call                  | 0    | 0    | 1    | 调用ad3                           |
| ret                   | 0    | 0    | 0    | 返回                              |
| setRet                | 0    | 0    | 1    | 放返回值                          |
| getRet                | 0    | 0    | 1    | 将返回值放入ad3                   |
| setArg                | 1    | 0    | 1    | 将ad3放入参数ad1,ad1为正整数            |
| getArg                | 1    | 0    | 1    | 将参数ad1放入ad3,ad1为正整数            |
| j                     | 0    | 0    | 1    | 跳转到ad3                         |
| readint               | 0    | 0    | 1    | 读取int放入ad3                    |
| readchar              | 0    | 0    | 1    | 读取char放入ad3                   |
| writeint              | 0    | 0    | 1    | 输出ad3                           |
| writechar             | 0    | 0    | 1    | 同上                              |
| writestring           | 0    | 0    | 1    | 同上                              |
| declareChar           | 0    | 0    | 1    | ad3为名字                         |
| declareInt            | 0    | 0    | 1    | 同上                              |
| declareString         | 0    | 0    | 1    | 同上                              |
| declareArrayInt       | 1    | 0    | 1    | ad3为名字 ad1为长度               |
| declareArrayChar      | 1    | 0    | 1    | ad3为名字 ad1为长度               |
| label                 | 0    | 0    | 1    | 在此处设置一个label               |
