#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "stdarg.h"
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "polarlog.h"

// global var

int log_level = LEVEL_DEBUG;


int  polarlog_init(const std::string &file, const std::string &dir)
{
	if (file.empty()) {
		std::cout << "file must be not null" << std::endl;
		return -1;
	}

	if (dir.empty()) {
		dir = "./";
	}

	_file = file;
	





}