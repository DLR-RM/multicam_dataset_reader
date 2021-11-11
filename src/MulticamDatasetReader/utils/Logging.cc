//
// Created by sewt_ma on 7/30/21.
//

#include "Logging.h"

#include <iostream>

using namespace MDR::Log;

log_level_value log_level = log_level_value::all;

void debug(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::debug){
		std::cout << "\u001b[30;1mDEBUG\u001b[0m: " << msg << line_ending;
	}
}
void info(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::info){
		std::cout << "\u001b[37;1mINFO\u001b[0m: " << msg << line_ending;
	}
}

void warn(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::warn){
		std::cout << "\u001b[33;1mWARN\u001b[0m: " << msg << line_ending;
	}
}

void error(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::error){
		std::cerr << "\u001b[31;1mERROR\u001b[0m: " << msg << line_ending;
	}
}