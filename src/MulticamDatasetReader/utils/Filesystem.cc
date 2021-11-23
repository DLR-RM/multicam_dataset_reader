//
// Created by sewt_ma on 11/9/21.
//

#include "Filesystem.h"

std::set<fs::path> MDR::get_sorted_directory_content(const fs::path& p) {
	std::set<fs::path> set;
	for(const auto& item : fs::directory_iterator(p)){
		set.insert(item.path());
	}
	return set;
}

std::vector<fs::path> MDR::get_dirs(const fs::path &p) {
	std::vector<fs::path> items;
	for(const auto& item : fs::directory_iterator(p)){
		if(fs::is_directory(item)){
			items.push_back(item);
		}
	}
	return items;
}

std::vector<fs::path> MDR::get_files(const fs::path &p) {
	std::vector<fs::path> items;
	for(const auto& item : fs::directory_iterator(p)){
		if(fs::is_regular_file(item)){
			items.push_back(item);
		}
	}
	return items;
}
