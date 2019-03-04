#ifndef _POLORLOG_FILE_APPENDER_H_
#define _POLORLOG_FILE_APPENDER_H_

#include <string>
#include <sys/time.h>
#include <cstring>
#include <pthread.h>
#include <fstream>
#include <stdarg.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <sys/stat.h>
#include "define.h"

namespace polarlog {

class FileAppender
{
public:
	FileAppender();
	~FileAppender();
	int init(std::string file, std::string dir);
	bool is_inited();
	int write_log(char *log, const char *format, va_list ap);
	int shift_log_file(time_t tt);
	int shift_log_if_need(time_t tt);
	int delete_old_log(time_t tt);
	void set_remain_day(int day);

private:
	std::fstream _fs;
	std::string _log_file;
	std::string _log_dir;
	std::string _log_path_file;
	bool _is_inited;
	int _remain_day;
	long _last_sec;
	pthread_mutex_t _rw_mutex;
};

}

#endif