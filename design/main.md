## 程序主入口main

main初始化Config类

## Config

Config类:
inputFileName string 输入文件名  
outputFileName string 输出文件名  
inputFile ifstream 输入文件 直接从里面读取char 打开 需判断文件是否存在 如不存在需要报错并退出程序  
outputFile ofstream 输出文件 打开  
argparser进行参数解析，参数：   
    file, 输入文件名
    --debug, open the debug mode  
    -o,--output, the output file name, the default is a.asm  
    其他视需要添加

## 词法分析

## 语法分析

## 语义分析

## 代码生成
