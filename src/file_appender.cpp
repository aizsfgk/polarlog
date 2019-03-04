
#include "file_appender.h"

namespace polarlog {

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

}