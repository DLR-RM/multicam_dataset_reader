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

MDR::CameraSensor::CameraSensor(fs::path frames_file,
								fs::path data_path,
								fs::path sensor_file,
								bool is_depth,
								bool is_lazy_load,
                                bool load) :
		Sensor<Image>(sensor_file.string()),
		m_frames_file_path(std::move(frames_file)),
		m_data_path(std::move(data_path)),
		m_sensor_file_path(std::move(sensor_file)),
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
	// check if frames file exists
	if(not fs::exists(m_frames_file_path)){
		Log::error("Could not find " + m_frames_file_path.string());
		return;
	}

	// open file
	std::ifstream fp(m_frames_file_path.string());

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
					m_frames_file_path.parent_path() / parts[1],
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
	if(not fs::exists(m_sensor_file_path)){
		Log::error("Could not find " + m_sensor_file_path.string());
		return;
	}

	// open file
	std::ifstream fp(m_sensor_file_path.string());

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
				m_pose = Utility::create_pose_matrix(px, py, pz, qx, qy, qz, qw);
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
