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