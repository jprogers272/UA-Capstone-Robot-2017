#include "pwm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>

using namespace std;

string identifyFilePWM(string prefix, int channel) {
	DIR *dir;
	struct dirent *dp;
	char file_end[20];
	dir = opendir(prefix.c_str());
	ostringstream return_path;
	while ( (dp = readdir(dir)) != NULL) {
		//printf("%s\n", dp->d_name);
		string entry(dp->d_name);
		if ( !entry.compare(".") || !entry.compare("..") ) {
			//do nothing
		}
		else {
			//name should be pwmchip#
			return_path << prefix << entry << "/pwm" << channel << '/';
		}
	}
	closedir(dir);
	return return_path.str();
}