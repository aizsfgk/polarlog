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

