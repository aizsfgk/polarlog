#ifndef _POLORLOG_H_
#define _POLORLOG_H_

#include <sys/time.h>
#include <pthread.h>
#include <cstring>
#include <string>
#include <fstream>

const int LEVEL_ERROR = 1
const int LEVEL_WARN  = 2
const int LEVEL_INFO  = 3
const int LEVEL_DEBUG = 4

extern int log_level;

// 宏实现
#define LOG_ERROR(format, args...) \
	polarlog_write(LEVEL_ERROR, __LINE__, __FILE__, format, args);

#define LOG_WARN(format, args...) \
	polarlog_write(LEVEL_WARN, __LINE__, __FILE__, format, args);

#define LOG_INFO(format, args...) \
	polarlog_write(LEVEL_INFO, __LINE__, __FILE__, format, args);

#define LOG_DEBUG(format, args...) \
	polarlog_write(LEVEL_DEBUG, __LINE__, __FILE__, format, args);


#define LOG(level, msg)  \
	LOG_##level("%s", msg) 




int  polarlog_init(const std::string &file, const std::string &dir);
void polarlog_write(int level, const int line, const char *file, const char *format, ...);
int  polarlog_set_level(std::string level_str);


class FileAppender
{
public:
	FileAppender();
	~FileAppender();
	int init(const std::string &file, const std::string &dir);
	bool is_inited();
	int write_log(char *log, const char *format, va_list ap);
	int shift_log_if_need(struct timeval tv, struct timezone tz);
	int delete_old_log(struct timeval tv);
	void set_remain_day(int day);

private:
	std::fstream _fs;
	std::string _log_file;
	std::string _log_dir;
	std::string _log_path_file;
	bool _is_inited;
	int _remain_day;
	long _last_sec;
};


#endif
