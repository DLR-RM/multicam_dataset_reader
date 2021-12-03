//
// Created by sewt_ma on 7/30/21.
//

#include "Logging.h"

#include <iostream>

using namespace MDR::Log;

log_level_value log_level = log_level_value::all;

void MDR::Log::set_log_level(log_level_value level) {
	log_level = level;
}

void MDR::Log::debug(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::debug){
		std::cout << "\u001b[30;1mDEBUG\u001b[0m: " << msg << line_ending;
	}
}
void MDR::Log::info(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::info){
		std::cout << "\u001b[37;1mINFO\u001b[0m: " << msg << line_ending;
	}
}

void MDR::Log::warn(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::warn){
		std::cout << "\u001b[33;1mWARN\u001b[0m: " << msg << line_ending;
	}
}

void MDR::Log::error(const message_type_t& msg, const std::string& line_ending){
	if(log_level >= log_level_value::error){
		std::cerr << "\u001b[31;1mERROR\u001b[0m: " << msg << line_ending;
	}
}