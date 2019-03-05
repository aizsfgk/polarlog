## 原理

### 相关技术

1. fstream
2. pthread_mutex_t
3. time


#### fstream

~~~
					  <-- basic_istream <-\
ios_base <- basic_ios                        basic_iostream <- basic_fstream
					  <-- basic_ostream <-/

~~~


### 设计

~~~
Polarlog 类
日志相关的操作

init 日志的初始化
set_log_level 设置日志级别
get_log_level
log  写入日志



FileAppender 类

文件相关的操作

~~~

### sapi 对接php扩展


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

