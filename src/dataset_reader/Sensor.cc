//
// Created by sewt_ma on 7/29/21.
//

#include "Sensor.h"
#include "Utils.h"
#include "Logging.h"

#include <algorithm>
#include <utility>
#include <fstream>
#include <string>
#include <iostream>

MD::Sensor::Sensor(std::string sensor_name):m_sensor_name(std::move(sensor_name)){}

MD::Sensor::Sensor(std::string sensor_name, const fs::path& path):
m_sensor_name(std::move(sensor_name)){
	load(path);
}

bool MD::Sensor::load(const fs::path& path){
	// start with sensor meta
	if(!parse_sensor_txt(path/"sensor.txt")){
		return false;
	}

	// look for rgb
	if(utils::check_for_directories(path, {"rgb"})){
		Log::debug("Found rgb directory");
		if(utils::check_for_files(path, {"frames_rgb.txt"})){
			m_has_rgb = true;
			Log::info("has rgb data");
			parse_frames_rgb_txt(path/"frames_rgb.txt");
		}
	}

	// look for depth
	if(utils::check_for_directories(path, {"depth"})){
		Log::debug("Found depth directory");
		if(utils::check_for_files(path, {"frames_depth.txt"})){
			m_has_depth = true;
			Log::info("has depth data");
			parse_frames_depth_txt(path/"frames_depth.txt");
		}
	}

	// look for imu data
	if(utils::check_for_files(path, {"gyro.txt"})){
		m_has_gyro = true;
		Log::info("has gyro data");
		parse_gyro_txt(path/"gyro.txt");
	}
	if(utils::check_for_files(path, {"accel.txt"})){
		m_has_accel = true;
		Log::info("has accel data");
		parse_accel_txt(path/"accel.txt");
	}

	return true;
}

bool MD::Sensor::parse_sensor_txt(const fs::path& path){
	Log::debug("Parse sensor.txt");
	auto f = std::ifstream(path);

	// process line by line
	bool is_pose_state = false;
	bool is_intr_state = false;
	for(std::string line; std::getline(f, line); ){
		if(!utils::str_empty_or_white(line)){
			if(utils::str_starts_with(line, "#")){
				// This is a comment. Might set state
				is_pose_state = utils::str_starts_with(line, "# Pose");
				is_intr_state = utils::str_starts_with(line, "# intrinsics");
			}
			else{
				if(is_pose_state){
					auto pose_elements = utils::str_split<double>(line, ';');
					m_extrinsic = utils::mat_get_posemat(pose_elements);
					is_pose_state = false;
				}
				if(is_intr_state){
					auto intr_elements = utils::str_split<double>(line, ';');
					is_intr_state = false;
				}
			}
		}
	}

	return true;
}

bool MD::Sensor::parse_frames_rgb_txt(const fs::path& path){
	Log::debug("Parse frames_rgb.txt");
	std::ifstream f(path);

	for(std::string line; std::getline(f, line);){
		if(!utils::str_starts_with(line, "#")){
			auto parts = utils::str_split<std::string>(line, ';');
			auto t     = std::stod(parts[0]);
			auto fp    = path/parts[1];
			m_data_rgb.emplace_hint(m_data_rgb.end(), t, fp);
			Log::debug("Added " + std::to_string(m_data_rgb.size()) + " RGB frames", "\r");
		}
	}
	Log::debug(std::to_string(m_data_rgb.size()) + " RGB frames added");

	m_it_rgb = m_data_rgb.begin();
	return true;
}

bool MD::Sensor::parse_frames_depth_txt(const fs::path& path){
	Log::debug("Parse frames_depth.txt");
	std::ifstream f(path);

	for(std::string line; std::getline(f, line);){
		if(!utils::str_starts_with(line, "#")){
			const auto parts = utils::str_split<std::string>(line, ';');
			const auto t     = std::stod(parts[0]);
			const auto fp    = path/parts[1];
			m_data_depth.emplace_hint(m_data_depth.end(), t, fp);
			Log::debug("Added " + std::to_string(m_data_depth.size()) + " Depth frames", "\r");
		}
	}
	Log::debug(std::to_string(m_data_depth.size()) + " Depth frames added");

	m_it_depth = m_data_depth.begin();
	return true;
}

bool MD::Sensor::parse_accel_txt(const fs::path& path){
	Log::debug("Parse accel.txt");
	std::ifstream f(path);

	for(std::string line; std::getline(f, line);){
		if(!utils::str_starts_with(line, "#")){
			const auto parts = utils::str_split<double>(line, ';');
			const auto& t     = parts[0];
			const auto& ax    = parts[1];
			const auto& ay    = parts[2];
			const auto& az    = parts[3];
			m_data_accel.emplace_hint(m_data_accel.end(), t, std::vector<double>{ax, ay, az});
			Log::debug("Added " + std::to_string(m_data_accel.size()) + " Accel frames", "\r");
		}
	}
	Log::debug(std::to_string(m_data_accel.size()) + " Accel frames added");

	m_it_accel = m_data_accel.begin();
	return true;
}

bool MD::Sensor::parse_gyro_txt(const fs::path& path){
	Log::debug("Parse gyro.txt");
	std::ifstream f(path);

	for(std::string line; std::getline(f, line);){
		if(!utils::str_starts_with(line, "#")){
			const auto parts = utils::str_split<double>(line, ';');
			const auto& t     = parts[0];
			const auto& ax    = parts[1];
			const auto& ay    = parts[2];
			const auto& az    = parts[3];
			m_data_gyro.emplace_hint(m_data_gyro.end(), t, std::vector<double>{ax, ay, az});
			Log::debug("Added " + std::to_string(m_data_gyro.size()) + " Gyro frames", "\r");
		}
	}
	Log::debug(std::to_string(m_data_gyro.size()) + " Gyro frames added");

	m_it_gyro = m_data_gyro.begin();
	return true;
}

double MD::Sensor::get_next_t(){
	// check if data is available
	if( m_it_rgb == m_data_rgb.end() and
	    m_it_depth == m_data_depth.end() and
	    m_it_accel == m_data_accel.end() and
	    m_it_gyro == m_data_gyro.end()){
		return std::numeric_limits<double>::infinity();
	}

	// update all times
	update_next_t();
	return std::min({m_next_t_rgb, m_next_t_depth, m_next_t_accel, m_next_t_gyro});
}

void MD::Sensor::update_next_t(){
	if(has_rgb() and m_it_rgb != m_data_rgb.end()){
		auto next_rgb   = m_it_rgb;
		std::advance(next_rgb, 1);
		if(m_it_rgb != m_data_rgb.end()) {
			m_next_t_rgb = next_rgb->first;
		}
		else{
			m_next_t_rgb = std::numeric_limits<double>::infinity();
		}
	}
	else{
		m_next_t_rgb = std::numeric_limits<double>::infinity();
	}

	if(has_depth() and m_it_depth != m_data_depth.end()){
		auto next_depth = m_it_depth;
		std::advance(next_depth, 1);
		if(m_it_depth != m_data_depth.end()) {
			m_next_t_depth = next_depth->first;
		}
		else{
			m_next_t_depth = std::numeric_limits<double>::infinity();
		}
	}
	else{
		m_next_t_depth = std::numeric_limits<double>::infinity();
	}

	if(has_accel() and m_it_accel != m_data_accel.end()){
		auto next_accel = m_it_accel;
		std::advance(next_accel, 1);
		if(m_it_accel != m_data_accel.end()) {
			m_next_t_accel = next_accel->first;
		}
		else{
			m_next_t_accel = std::numeric_limits<double>::infinity();
		}
	}
	else{
		m_next_t_accel = std::numeric_limits<double>::infinity();
	}

	if(has_gyro() and m_it_gyro != m_data_gyro.end()){
		auto next_gyro = m_it_gyro;
		std::advance(next_gyro, 1);
		if(m_it_gyro != m_data_gyro.end()) {
			m_next_t_gyro = next_gyro->first;
		}
		else{
			m_next_t_gyro = std::numeric_limits<double>::infinity();
		}
	}
	else{
		m_next_t_gyro = std::numeric_limits<double>::infinity();
	}

}

MD::Sensor::sensor_frame_t MD::Sensor::get_active_frame() const{
	std::optional<RGBData> opt_rgb;
	if((m_it_rgb->first - get_current_t()) <= TIME_DIFFERENCE_FOR_ACTIVE){
		opt_rgb = m_it_rgb->second;
	}

	std::optional<DepthData> opt_depth;
	if((m_it_depth->first - get_current_t()) <= TIME_DIFFERENCE_FOR_ACTIVE){
		opt_depth = m_it_depth->second;
	}

	std::optional<AccelData> opt_accel;
	if((m_it_accel->first - get_current_t()) <= TIME_DIFFERENCE_FOR_ACTIVE){
		opt_accel = m_it_accel->second;
	}

	std::optional<GyroData> opt_gyro;
	if((m_it_gyro->first - get_current_t()) <= TIME_DIFFERENCE_FOR_ACTIVE){
		opt_gyro = m_it_gyro->second;
	}

	return {opt_rgb, opt_depth, opt_accel, opt_gyro};
}

void MD::Sensor::advance(double t){
	if(t <= 0){
		t = get_next_t();
	}

	// set to new t
	m_current_t = t;

	// check if any iterators are now in the past
	if(has_rgb() and m_it_rgb != m_data_rgb.end() and m_it_rgb->first - TIME_DIFFERENCE_FOR_ACTIVE < t){
		std::advance(m_it_rgb, 1);
	}
	if(has_depth() and m_it_depth != m_data_depth.end() and m_it_depth->first - TIME_DIFFERENCE_FOR_ACTIVE < t){
		std::advance(m_it_depth, 1);
	}
	if(has_accel() and m_it_accel != m_data_accel.end() and m_it_accel->first - TIME_DIFFERENCE_FOR_ACTIVE < t){
		std::advance(m_it_accel, 1);
	}
	if(has_gyro() and m_it_gyro != m_data_gyro.end() and m_it_gyro->first - TIME_DIFFERENCE_FOR_ACTIVE < t){
		std::advance(m_it_gyro, 1);
	}

	// update next t
	update_next_t();
}
