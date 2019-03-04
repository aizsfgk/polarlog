#include "config.h"
#include <map>
#include <sstream>
#include <fstream>



namespace polarlog {

/*
	const char *config_file
 */
int parse_config_to_map(const char *config_file, std::map<std::string, std::string> &config_map)
{
	
	std::ifstream fs(config_file);
	if (!fs.is_open()) {
		return -1;
	}

	while(fs.good()) {
		std::string line;
		std::getline(fs, line);
		if (line[0] == '#') {
			continue;
		}
		std::string key, val;
		std::stringstream ss;
		ss << line;
		std::getline(ss, key, '=');
		std::getline(ss, val, '\n');
		config_map[key] = val;
	}

	fs.close();
	return 0;
}

}