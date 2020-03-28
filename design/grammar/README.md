grammar.zh为原始文法
grammar.en为原始文法的英文版以及根据python中正则表达式语法修改的正则表达式
grammar为修改后的文法，主要增加以下内容:
+ 增加switch中的default
+ 表达式中的+/-改为可选项
+ **还有一些保留字，类似于break/continue/&&/||这种是否要支持，容我三思**
+ integer中删除0