//
// Created by sewt_ma on 11/23/21.
//

#include "InertialSensor.h"

#include <MulticamDatasetReader/utils/Logging.h>
#include <MulticamDatasetReader/utils/Misc.h>

#include <fstream>
#include <sstream>
#include <string>
#include <utility>

using namespace MDR;

InertialSensor::InertialSensor(std::string sensor_name,
							   fs::path sensor_root,
							   bool is_accel,
							   bool load) :
		Sensor<InertiaMeasurement>(
				std::move(sensor_name),
				std::move(sensor_root)
				),
        m_is_accel(is_accel),
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
	if(not fs::exists(get_frames_file())){
		Log::error("Could not find " + get_frames_file().string());
		return;
	}

	// open file
	std::ifstream fp(get_frames_file().string());

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
			this->insert(t,{ax,ay,az});
		}
	}
	Log::debug("Created "
		+ std::to_string(size())
		+ " data entries");

	// close file
	fp.close();
}

bool InertialSensor::check_if_sensor_exists(const boost::filesystem::path &path, bool is_accel) {
	if(is_accel){
		if(not fs::exists(path/Globals::frames_file_accel)){
			return false;
		}
	}
	else{
		if(not fs::exists(path/Globals::frames_file_gyro)){
			return false;
		}
	}

	return true;
}
