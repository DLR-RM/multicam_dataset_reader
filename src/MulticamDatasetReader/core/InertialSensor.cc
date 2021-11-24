//
// Created by sewt_ma on 11/23/21.
//

#include "InertialSensor.h"

#include <MulticamDatasetReader/utils/Logging.h>
#include <MulticamDatasetReader/utils/Misc.h>

#include <fstream>
#include <sstream>
#include <string>

using namespace MDR;

MDR::InertialSensor::InertialSensor(fs::path frames_file,
									fs::path sensor_file,
									bool load):
						m_frames_file_path(std::move(frames_file)),
						m_sensor_file_path(std::move(sensor_file)),
						m_is_loaded(false){
	if(load){
		this->load();
	}
}

void MDR::InertialSensor::load() {
	if(is_loaded()){
		Log::debug("Already loaded");
		return;
	}

	parse_frames_file();

	m_is_loaded = true;
}

void MDR::InertialSensor::parse_frames_file() {
	// check for frames file
	if(not fs::exists(m_frames_file_path)){
		Log::error("Could not find " + m_frames_file_path.string());
		return;
	}

	// open file
	std::ifstream fp(m_frames_file_path.string());

	// read line-by-line
	std::string line;
	while(std::getline(fp, line)){
		// check for comments
		if(line.rfind('#', 0) != 0 and not line.empty()){
			// split line
			const auto& parts = Utility::split(line, ';');

			// convert to doubles
			double t  = std::stod(parts[0]);
			double ax = std::stod(parts[1]);
			double ay = std::stod(parts[2]);
			double az = std::stod(parts[3]);

			// insert
			m_data.emplace_back(t,std::vector<double>({ax,ay,az}));
		}
	}
	Log::debug("Created "
		+ std::to_string(m_data.size())
		+ " data entries");

	// close file
	fp.close();
}
