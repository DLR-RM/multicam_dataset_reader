//
// Created by sewt_ma on 7/30/21.
//

#ifndef MULTICAM_DATASET_LOGGING_H
#define MULTICAM_DATASET_LOGGING_H

#include <string>
using message_type_t = std::string;

namespace MDR{
namespace Log{

enum class log_level_value{
	error = 1,
	warn  = 2,
	info  = 3,
	debug = 4,
	all   = 5
};

void debug(const message_type_t& msg, const std::string& line_ending="\n\r");

void info(const message_type_t& msg, const std::string& line_ending="\n\r");

void warn(const message_type_t& msg, const std::string& line_ending="\n\r");

void error(const message_type_t& msg, const std::string& line_ending="\n\r");


}
}
#endif //MULTICAM_DATASET_LOGGING_H
