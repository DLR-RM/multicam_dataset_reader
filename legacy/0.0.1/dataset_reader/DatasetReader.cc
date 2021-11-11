//
// Created by sewt_ma on 2/17/21.
//

#include "DatasetReader.h"
#include "Logging.h"
#include "Utils.h"

#include <algorithm>
#include <fstream>

using namespace MD;

DatasetReader::DatasetReader(const fs::path& dataset_root):m_dataset_root(dataset_root){
	// check path for dataset
	Log::info("Validate dataset at " + dataset_root.string());
	if(is_valid_dataset(m_dataset_root)){
		// path points to valid dataset
		load(m_dataset_root);
	}
	else{
		// path points to invalid dataset
		Log::error("Could not load dataset. Dataset is not valid");
	}
}

void DatasetReader::load(const fs::path& root){
	// read meta.txt
	Log::info("Load dataset at " + root.string());
	std::tie(m_dataset_name, m_dataset_description) =
	        parse_meta_txt(root/"meta.txt");

	// load all sensors
	for(const auto& sensor_dir : utils::get_all_of_type(root, utils::fs_type::dir)){
		Log::info("Load sensor " + sensor_dir);
		m_sensors.emplace_back(sensor_dir, root/sensor_dir);
	}

	m_is_init = true;
}

bool DatasetReader::is_valid_dataset(const fs::path& root){
	// starting at root level.
	if(!utils::check_for_files(root, {"meta.txt"})){
		return false;
	}

	// now check every directory
	for(const auto& sensor_dir : utils::get_all_of_type(root, utils::fs_type::dir)){
		if(!utils::check_for_files(root/sensor_dir, {"sensor.txt"})){
			return false;
		}

		// check for rgb sensor
		if(utils::check_for_directories(root/sensor_dir, {"rgb"})){
			if(!utils::check_for_files(root/sensor_dir, {"frames_rgb.txt"})){
				return false;
			}
		}

		// check for depth sensor
		if(utils::check_for_directories(root/sensor_dir, {"depth"})){
			if(!utils::check_for_files(root/sensor_dir, {"frames_depth.txt"})){
				return false;
			}
		}
	}

	return true;
}

std::pair<std::string, std::string> DatasetReader::parse_meta_txt(const fs::path& file_path){
	Log::debug("Parse meta.txt");
	auto file = std::ifstream(file_path);
	bool first_line = true;
	std::string name, description;
	for(std::string line; std::getline(file, line); ){
		if(first_line){
			name = line;
			first_line = false;
		}
		else{
			description.append(line);
		}
	}

	return {name, description};
}

double DatasetReader::get_next_t(){
	std::vector<double> next_ts;
	for(auto& sensor : m_sensors){
		next_ts.emplace_back(sensor.get_next_t());
	}

	const auto t = *(std::min_element(next_ts.begin(), next_ts.end()));
	return t;
}

void DatasetReader::advance(double t){
	if(t<0){
		t = get_next_t();
	}

	m_current_t = t;
	for(auto& sensor : m_sensors){
		sensor.advance(m_current_t);
	}
}
