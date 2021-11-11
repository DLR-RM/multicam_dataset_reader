//
// Created by sewt_ma on 7/28/21.
//

#include "Utils.h"
#include "Logging.h"

#include <iostream>
#include <algorithm>

bool MD::utils::check_for_directories(const fs::path& root, const std::vector<std::string>& dirs){
	return check_for(root, dirs, fs_type::dir);
}

bool MD::utils::check_for_files(const fs::path& root, const std::vector<std::string>& files){
	return check_for(root, files, fs_type::file);
}

bool MD::utils::check_for(const fs::path& root, const std::vector<std::string>& items, MD::utils::fs_type type){
	const auto dir_content = get_all_of_type(root, type);
	// if no items were found
	if(dir_content.empty()){
		if(items.empty()){
			return true;
		}
		else{
			Log::error(root.string() + " is empty");
			return false;
		}
	}

	for(const auto& item : items){
		// look if item is in dir_content
		if(std::find(dir_content.begin(), dir_content.end(), item) == dir_content.end()){
			Log::debug("Could not find '" + item + "'. Expected it at " + root.string());
			return false;
		}
	}

	// if we reach this point, everything was found
	return true;
}

MD::utils::fs_type MD::utils::get_type(const fs::path& path){
	if(fs::is_directory(path)){
		return fs_type::dir;
	}
	if(fs::is_regular_file(path)){
		return fs_type::file;
	}

	return fs_type::other;
}

std::vector<std::string> MD::utils::get_all_of_type(const fs::path& path, MD::utils::fs_type type){
	std::vector<std::string> items;
	if(fs::exists(path)){
		if(fs::is_directory(path)){
			for(const auto& item : fs::directory_iterator(path)){
				if(get_type(item.path().string()) == type){
					items.push_back(item.path().filename());
				}
			}
		}
	}
	return items;
}

bool MD::utils::str_starts_with(const std::string& str, const std::string& test){
	return str.rfind(test, 0) == 0;
}

bool MD::utils::str_empty_or_white(const std::string& str){
	// trivial empty str
	if(str.empty()){
		return true;
	}

	// check all for whitespace characters
	return std::all_of(str.begin(), str.end(), [](const char& c){
		if(c == ' ' or c == '\n' or c == '\t' or c == '\r'){
			return true;
		}
		return false;
	});
}


Eigen::Matrix3d MD::utils::mat_quat2pose(const Eigen::Quaterniond& quats){
	return quats.matrix();
}

Eigen::Matrix3d MD::utils::mat_quat2pose(std::vector<double>& quats_vec){
	if(quats_vec.size() != 4){
		Log::error("Expected 4 values.");
		return Eigen::Matrix3d();
	}

	Eigen::Quaterniond quats(
			quats_vec[3], // Important: this is qw!
			quats_vec[0],
			quats_vec[1],
			quats_vec[2]
			);
	return mat_quat2pose(quats);
}

Eigen::Matrix4d MD::utils::mat_get_posemat(std::vector<double>& elements_vec){
	Eigen::Matrix4d pose = Eigen::Matrix4d::Identity();

	pose.block(0,0,3,3) = mat_quat2pose({
		elements_vec[3], // qx
		elements_vec[4], // qy
		elements_vec[5], // qz
		elements_vec[6]  // qw
	});
	pose(0,3) = elements_vec[0];
	pose(1,3) = elements_vec[1];
	pose(2,3) = elements_vec[2];

	return pose;
}
