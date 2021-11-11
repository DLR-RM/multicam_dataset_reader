//
// Created by sewt_ma on 7/30/21.
//

#ifndef MULTICAM_DATASET_LOGGING_H
#define MULTICAM_DATASET_LOGGING_H

#include <string>
#include <string_view>

namespace MD::Log{

enum class log_level_value{
	error = 1,
	warn  = 2,
	info  = 3,
	debug = 4,
	all   = 5
};

void debug(const std::string_view& msg, const std::string& line_ending="\n\r");

void info(const std::string_view& msg, const std::string& line_ending="\n\r");

void warn(const std::string_view& msg, const std::string& line_ending="\n\r");

void error(const std::string_view& msg, const std::string& line_ending="\n\r");


}

#endif //MULTICAM_DATASET_LOGGING_H
