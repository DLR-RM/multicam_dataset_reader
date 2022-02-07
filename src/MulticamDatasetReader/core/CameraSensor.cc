//
// Created by sewt_ma on 11/23/21.
//

#include "CameraSensor.h"

#include <MulticamDatasetReader/utils/Logging.h>
#include <MulticamDatasetReader/utils/Misc.h>

#include <utility>
#include <fstream>
#include <sstream>
#include <string>

MDR::CameraSensor::CameraSensor(std::string sensor_name,
								fs::path sensor_root_path,
								bool is_depth,
								bool is_lazy_load,
                                bool load) :
		Sensor<Image>(
				std::move(sensor_name),
				std::move(sensor_root_path)
				),
		m_is_depth(is_depth),
		m_is_lazy_load(is_lazy_load),
		m_is_loaded(false)
								{
	// load data if requested
	if(load){
		this->load();
	}
}

void MDR::CameraSensor::load() {
	// stop if already loaded
	if(is_loaded()){
		return;
	}

	parse_frames_file();
	parse_sensor_file();

	m_is_loaded = true;
}

void MDR::CameraSensor::parse_frames_file() {
	// check for frames file and data path
	if(not fs::exists(get_frames_file())){
		Log::error("Could not find " + get_frames_file().string());
		std::exit(1);
	}
	if(not fs::exists(get_data_path())
	   and not fs::is_directory(get_data_path())){
		Log::error("Could not find directory " + get_data_path().string());
		std::exit(1);
	}

	// open file
	std::ifstream fp(get_frames_file().string());

	// read file line-by-line
	std::string line;
	while(std::getline(fp, line)){
		// check that the line does not start with '#'
		if(line.rfind('#', 0) != 0 and not line.empty()){
			// split lines
			const auto& parts = Utility::split(line, ';');
			if(parts.size() != 2){
				Log::warn("Expected exactly 2 elements. Got "
				+ std::to_string(parts.size()));
				continue;
			}

			// create data entry
			Image img = Image(
					get_path() / parts[1],
					m_is_depth,
					m_is_lazy_load);
			this->insert(std::stod(parts[0]), img);
		}
	}

	Log::debug("Created "
		+ std::to_string(this->size())
		+ " data entries");

	// close file
	fp.close();
}

void MDR::CameraSensor::parse_sensor_file() {
	// check if sensor file exists
	if(not fs::exists(get_sensor_file())){
		Log::warn("Could not find " + get_sensor_file().string());
		return;
	}

	// open file
	std::ifstream fp(get_sensor_file().string());

	// read line-by-line
	std::string line;
	while(std::getline(fp, line)){
		// skip comments and empty lines
		if(line.rfind('#', 0) != 0 and not line.empty()){
			const auto& parts = Utility::split(line, ';');

			// pose has 7 elements
			if(parts.size() == 7){
				double px = std::stod(parts[0]);
				double py = std::stod(parts[1]);
				double pz = std::stod(parts[2]);
				double qx = std::stod(parts[3]);
				double qy = std::stod(parts[4]);
				double qz = std::stod(parts[5]);
				double qw = std::stod(parts[6]);
				set_pose(Utility::create_pose_matrix(px, py, pz, qx, qy, qz, qw));
			}

			// intrinsics have 4 or 5 elements
			if(parts.size() == 4){
				float fx = std::stof(parts[0]);
				float fy = std::stof(parts[1]);
				float cx = std::stof(parts[2]);
				float cy = std::stof(parts[3]);
				m_intrinsics = Utility::create_intrinsics(fx, fy, cx, cy);
			}
			if(parts.size() == 5){
				float fx = std::stof(parts[0]);
				float fy = std::stof(parts[1]);
				float cx = std::stof(parts[2]);
				float cy = std::stof(parts[3]);
				float s  = std::stof(parts[4]);
				m_intrinsics = Utility::create_intrinsics(fx, fy, cx, cy, s);
			}
		}
	}

	// close file
	fp.close();
}

bool MDR::CameraSensor::check_if_sensor_exists(const fs::path &path,
											   bool is_depth) {
	if(is_depth){
		if(not fs::exists(path/Globals::frames_file_depth)){
			return false;
		}
		if(not fs::exists(path/Globals::data_path_depth)){
			return false;
		}
	}
	else{
		if(not fs::exists(path/Globals::frames_file_rgb)){
			return false;
		}
		if(not fs::exists(path/Globals::data_path_rgb)){
			return false;
		}
	}
	return true;
}

