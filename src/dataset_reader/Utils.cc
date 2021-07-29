//
// Created by sewt_ma on 7/28/21.
//

#include "Utils.h"

#include <iostream>

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
			std::cerr << root.string() << " is empty";
			return false;
		}
	}

	for(const auto& item : items){
		// look if item is in dir_content
		if(std::find(dir_content.begin(), dir_content.end(), item) == dir_content.end()){
			std::cerr << "Could not find " << item << ". Expected it at " << root.string() << std::endl;
			return false;
		}
	}

	// if we reach this point, everything was found
	return true;
}

MD::utils::fs_type MD::utils::get_type(fs::path path){
	if(fs::is_directory(path)){
		return fs_type::dir;
	}
	if(fs::is_regular_file(path)){
		return fs_type::file;
	}

	return fs_type::other;
}

std::vector<fs::path> MD::utils::get_all_of_type(const fs::path& path, MD::utils::fs_type type){
	std::vector<fs::path> items;
	if(fs::is_directory(path)){
		for(const auto& item : fs::directory_iterator(path)){
			if(get_type(item) == type){
				items.push_back(item);
			}
		}
	}
	return items;
}
