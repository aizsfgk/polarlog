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
			return -1;
		}
		_file_appender->init(_config_map["log_file"], _config_map["log_dir"]);
		const char *level_str = const_cast<char *>(_config_map["level"].c_str());
		const int level = get_log_level(level_str);
		set_log_level(level);
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
		//printf("level: %d ===> _log_level: %d\n", level, _log_level);

		//std::string fin_format = "[%s][%s][%s/%d]" + format;


		std::string level_str = get_log_level_strw(level);
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
			_file_appender->shift_log_if_need(tt);
			_file_appender->write_log(log_str, fin_format.str().c_str(), ap);
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

std::string Polarlog::get_log_level_strw(int level)
{
	if (level == LEVEL_DEBUG)  {
		return "DEBUG";
	} else if (level == LEVEL_INFO) {
		return "INFO ";
	} else if (level == LEVEL_WARN) {
		return "WARN ";
	} else if (level == LEVEL_ERROR) {
		return "ERROR";
	} else {
		return "NOT KNOW";
	}
}

} // end namespace polarlog


// =============== API =============== //

polarlog::Polarlog *gp_logger;
polarlog::FileAppender gp_file_appender;

// TODO
int polarlog_init(const std::string &file, const std::string &dir)
{
	gp_logger = new polarlog::Polarlog(&gp_file_appender);
	gp_logger->init(file, dir);
	return 0;
}

void polarlog_write(int level, int line, const char *file, const char *format, ...)
{
	// printf("level=>%d, line=>%d, file=>%s\n", level, line, file);

	
	if (level < LEVEL_ERROR) {
		return;
	}

	va_list ap;
	va_start(ap, format);
	gp_logger->log(level, line, file, format, ap);
	va_end(ap);
}

void polarlog_set_level(const char *level)
{
	int l = gp_logger->get_log_level(level);
	gp_logger->set_log_level(l);
}

const char *polarlog_get_level()
{
	int l = gp_logger->get_log_level();
	std::string ls = gp_logger->get_log_level_str(l);
	return ls.c_str();
}

void polarlog_end()
{
	delete gp_logger;
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