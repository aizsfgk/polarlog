## polarlog

polarlog是用C++编写的极简的日志库，代码精简，并且线程安全。


### 安装

~~~bash

# 1. 执行脚本
./ext_build.sh

~~~


### 使用文档

> php 接口函数


~~~php
// 函数

// 获取polarphp版本
string polarlog_version();
// 设置日志级别
bool polarlog_set_level(string $level);
// 获取日志级别
string polarlog_get_level();
// 打印日志
int PLOG(string $level, string $msg);
// 打印错误日志
int PLOG_ERROR(string $msg);
// 打印警告日志
int PLOG_WARN(string $msg);
// 打印信息日志
int PLOG_INFO(string $msg);
// 打印调试日志
int PLOG_DEBUG(string $msg);

~~~

> c/c++ 库函数

~~~cpp
// 初始化
// TODO
int polarlog_init(const std::string &file, const std::string &dir);
// 写入日志
void polarlog_write(int level, int line, const char *file, const char *format, ...);
// 设置日志级别
void polarlog_set_level(const char *level);
// 获取日志级别
const char *polarlog_get_level();
// 日志结束
void polarlog_end();
~~~

### 问题



