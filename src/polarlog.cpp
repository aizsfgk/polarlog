#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "polarlog.h"




namespace polarlog {

Polarlog::~Polarlog()
{
}

int Polarlog::init(const std::string &conf_file, const std::string &conf_dir)
{
	_config_file = conf_file;
	_config_dir = conf_dir;

	signal(SIGUSR1, polarlog_reload);

    if (_check_config_file() != 0) {
    	printf("FATAL: _check_config_file error\n");
    }

	if (_is_use_fa && !_config_map.empty()) {

		if (_config_map.find("log_file") == _config_map.end()) {
			printf("FALTAL: log_file field is null\n");
		}
		_file_appener->init(_config_map["log_file"], _config_map["log_dir"]);
	} else {
		printf("FATAL: _config_map is empty\n");
	}
	return 0;
}

int Polarlog::_check_config_file()
{
	if (_config_file.empty()) {
		return -1;
	}
	if (_config_dir.empty()) {
		_config_dir = ".";
	}

	std::string config_path_file = _config_dir + "/" + _config_file;

	if (access(config_path_file.c_str(), F_OK) != 0) {
		printf("SPECIAL-FATAL: config_file is not exist\n");
		return 0;
	}

	parse_config_to_map(config_path_file.c_str(), _config_map);
	return 0;
}

void Polarlog::log(int level, int line, const char *file, const char *format, va_list ap)
{
	if (level <= _log_level) {

		//std::string fin_format = "[%s][%s][%s/%d]" + format;
		printf("_log_level: %d\n", _log_level);

		std::string level_str = get_log_level_str(level);
		time_t tt = time(NULL);
		std::string time = get_now_time(tt);

		std::stringstream fin_format;

		 fin_format << "[" << level_str << "] " << "[" << time << "]" << "[" << file << ":" << line << "] " << format;

		if (!_is_use_fa) {
			printf(fin_format.str().c_str(), ap);
			printf("\n");
			return;
		} else {
			char log_str[LOG_MAX_SIZE];
			bzero(log_str, 0);
			_file_appener->shift_log_if_need(tt);
			_file_appener->write_log(log_str, fin_format.str().c_str(), ap);
		}
	}
}

void Polarlog::set_log_level(int level)
{
	_log_level = level;
}

bool Polarlog::get_use_fa()
{
	return  _is_use_fa;
}

int Polarlog::get_log_level()
{
	return _log_level;
}

int Polarlog::get_log_level(const char *level_str)
{
	if ( strcasecmp(level_str, "ERROR") == 0) {
		return LEVEL_ERROR;
	} else if( strcasecmp(level_str,"WARN") == 0) {
		return LEVEL_WARN;
	} else if ( strcasecmp(level_str,"INFO") == 0) {
		return LEVEL_INFO;
	} else if ( strcasecmp(level_str, "DEBUG") == 0) {
		return LEVEL_DEBUG;
	} else {
		return LEVEL_INFO;
	}
}

std::string Polarlog::get_now_time(time_t tt)
{
	struct tm *now_tm;
	now_tm = localtime(&tt);

	char now_time_str[100];
	bzero(now_time_str, 0);
	// %04d-%02d-%02d %02d:%02d:%02d.%03d
	sprintf(now_time_str, "%04d-%02d-%02d %02d:%02d:%02d",
		now_tm->tm_year+1900, now_tm->tm_mon+1, now_tm->tm_mday, now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);

	return std::string(now_time_str);
}

std::string Polarlog::get_log_level_str(int level)
{
	if (level == LEVEL_DEBUG)  {
		return "DEBUG";
	} else if (level == LEVEL_INFO) {
		return "INFO";
	} else if (level == LEVEL_WARN) {
		return "WARN";
	} else if (level == LEVEL_ERROR) {
		return "ERROR";
	} else {
		return "NOT KNOW";
	}
}





// ================= FileAppender =================== // 

FileAppender::FileAppender()
{

}
int FileAppender::init(std::string file, std::string dir)
{
	if (file.empty()) {
		printf("FATAL: file is null\n");
		return -1;
	}

	if (dir.empty()) {
		dir = ".";
	} else {
		int ret = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (ret != 0 && errno != EEXIST) {
			printf("FATAL: mdir %s error\n", dir.c_str());
			return -1;
		}
	}

	_log_file = file;
	_log_dir  = dir;
	_log_path_file = _log_dir + "/" + _log_file;

	_fs.open(_log_path_file.c_str(), std::fstream::out | std::fstream::app);
	_is_inited = true;
	pthread_mutex_init(&_rw_mutex, NULL);

	return 0;
}


FileAppender::~FileAppender()
{
	if (_fs.is_open()) {
		_fs.close();
	}
}

bool FileAppender::is_inited()
{
	return _is_inited;
}

void FileAppender::set_remain_day(int day)
{
	_remain_day = day;
}

int FileAppender::write_log(char *log, const char *format, va_list ap)
{
	pthread_mutex_lock(&_rw_mutex);

	if (_fs.is_open()) {
		vsnprintf(log, LOG_MAX_SIZE - 1, format, ap);
		_fs << log << "\n";
		_fs.flush();
	}

	pthread_mutex_unlock(&_rw_mutex);
	return 0;
}


int FileAppender::shift_log_file(time_t tt)
{
	long timestamp = tt - ONE_DAY_SECONDS;
	struct tm *y_tm;

	y_tm = localtime(&timestamp);
	char new_file_name[100];
	sprintf(new_file_name, "%s.%04d-%02d-%02d", _log_path_file.c_str(), 
		y_tm->tm_year + 1900, y_tm->tm_mon + 1, y_tm->tm_mday);


	std::string new_path_file = _log_dir + "/" + new_file_name;

	// 文件不存在
	if (access(new_path_file.c_str(), F_OK) != 0) {
		rename(_log_path_file.c_str(), new_path_file.c_str());
		_fs.close();
		_fs.open(_log_path_file.c_str(), std::fstream::out | std::fstream::app);
	}

	return 0;
}

int FileAppender::shift_log_if_need(time_t tt)
{
	if (_last_sec == 0) {
		_last_sec = tt;
	}

	long now_sec = tt;
	long yesterday_sec =  _last_sec;

	if (now_sec - yesterday_sec >= ONE_DAY_SECONDS) {
		pthread_mutex_lock(&_rw_mutex);

		if (!shift_log_file(tt)) {
			printf("FATAL: shift log file error\n");
			return -1;
		}

		pthread_mutex_lock(&_rw_mutex);

		delete_old_log(tt);
	}
	return 0;
}

/**
 * 删除旧日志
 */
int FileAppender::delete_old_log(time_t tt)
{
	if (_remain_day <= 0) {
		return 0;
	}
	long timestamp = tt - _remain_day * ONE_DAY_SECONDS;

	struct tm *remain_tm;
	remain_tm = localtime(&timestamp);

	char delete_old_file[100];
	// "%04d-%02d-%02d %02d:%02d:%02d.%03d"
	sprintf(delete_old_file, "%s.%04d-%02d-%02d", _log_path_file.c_str(), 
		remain_tm->tm_year+1990, remain_tm->tm_mon+1, remain_tm->tm_mday);

	std::string delete_old_path_file = _log_dir + "/" + delete_old_file;

	if (access(delete_old_path_file.c_str(), F_OK) == 0) {
		return remove(delete_old_path_file.c_str());
	}
	return 0;
}



} // end namespace polarlog


// extern polarlog::Polarlog gp_logger;
polarlog::FileAppender gp_file_appender;
polarlog::Polarlog *gp_logger;


int polarlog_init(const std::string &file, const std::string &dir)
{
	// polarlog::FileAppender gp_file_appender = new polarlog::FileAppender();
	polarlog::Polarlog *gp_logger = new polarlog::Polarlog(&gp_file_appender);
	gp_logger->init(file, dir);
	return 0;
}

void polarlog_write(int level, int line, const char *file, const char *format, ...)
{
	if (level < LEVEL_ERROR) {
		return;
	}

	va_list ap;
	va_start(ap, format);

	gp_logger->log(level, line, file, format, ap);

	//_log(format, ap);

	va_end(ap);
}

void polarlog_set_level(const char *level)
{
	int l = gp_logger->get_log_level(level);
	gp_logger->set_log_level(l);
}

void polarlog_end()
{
	delete gp_logger;
	//delete gp_file_appender;
	return;
}


void polarlog_reload(int sig)
{
	printf("receive sig: %d\n", sig);
	if (!gp_logger) {
		printf("FATAL: gp_logger not init\n");
	} else {
		gp_logger->_check_config_file();
	}
}

