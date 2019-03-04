#ifndef _POLARLOG_CONFIG_H_
#define _POLARLOG_CONFIG_H_

#include <sstream>
#include <map>
#include <string>
#include <cstring>
#include <iostream>

namespace polarlog {

int parse_config_to_map(const char *config_file, std::map<std::string, std::string> &config_map);

}

#endif