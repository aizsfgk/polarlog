#ifndef _POLORLOG_H_
#define _POLORLOG_H_

#include <sys/time.h>
#include <pthread.h>
#include <cstring>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <stdio.h>
#include "file_appender.h"
#include "define.h"
#include "config.h"

const int LEVEL_ERROR = 1;
const int LEVEL_WARN  = 2;
const int LEVEL_INFO  = 3;
const int LEVEL_DEBUG = 4;

// 宏实现
#define LOG_ERROR(format, args...) \
	polarlog_write(LEVEL_ERROR, __LINE__, __FILE__, format, ##args);

#define LOG_WARN(format, args...) \
	polarlog_write(LEVEL_WARN, __LINE__, __FILE__, format, ##args);

#define LOG_INFO(format, args...) \
	polarlog_write(LEVEL_INFO, __LINE__, __FILE__, format, ##args);

#define LOG_DEBUG(format, args...) \
	polarlog_write(LEVEL_DEBUG, __LINE__, __FILE__, format, ##args);


#define LOG(level, msg)  \
	LOG_##level("%s", msg) 

#define LOG_COMMON(level, line, file, format, args...) \
	polarlog_write(level, line, file, format, ##args);

extern "C" {

int  polarlog_init(const std::string &file, const std::string &dir);
void polarlog_write(int level, int line, const char *file, const char *format, ...);
void polarlog_set_level(const char *level);
const char *polarlog_get_level();
void polarlog_end();
void polarlog_reload(int sig);

}

namespace polarlog {

class Polarlog
{
public:
	Polarlog() : 
		_file_appender(NULL),_log_level(LEVEL_DEBUG),_config_file(""),_is_use_fa(false) {} 
	Polarlog(FileAppender *fa) : 
		_file_appender(fa),_log_level(LEVEL_DEBUG),_config_file(""),_is_use_fa(true) {} 
	~Polarlog();
	int init(const std::string &file, const std::string &dir);
	void log(int level, int line, const char *file, const char *format, va_list ap);
	void set_log_level(int level);
	int get_log_level();
	int get_log_level(const char *level_str);
	std::string get_log_level_str(int level);
	std::string get_now_time(time_t tt);
	bool get_use_fa();
	int _check_config_file();

private:
	
	FileAppender *_file_appender;
	std::map<std::string, std::string> _config_map;
	int _log_level;
	std::string _config_file;
	std::string _config_dir;
	bool _is_use_fa;

};

}

#endif