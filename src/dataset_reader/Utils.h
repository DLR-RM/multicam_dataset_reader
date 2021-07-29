//
// Created by sewt_ma on 7/28/21.
//

#ifndef MULTICAM_DATASET_UTILS_H
#define MULTICAM_DATASET_UTILS_H

#include "Filesystem.h"

#include <vector>
#include <string>

namespace MD{
namespace utils{

/**
 * Check that all directories in dirs exists
 * \param root folder to check
 * \param dirs list of names to check
 * \return True if all dirs were found
 */
bool check_for_directories(const fs::path& root,
						   const std::vector<std::string>& dirs);

/**
 * Check that all files in files exists
 * \param root folder to check
 * \param files list of names to check
 * \return True if all files were found
 */
bool check_for_files(const fs::path& root,
					 const std::vector<std::string>& files);

enum class fs_type{
	dir,
	file,
	other
};
bool check_for(const fs::path& root,
			   const std::vector<std::string>& items,
			   fs_type type);

fs_type get_type(fs::path path);

std::vector<fs::path> get_all_of_type(const fs::path& path, fs_type type);



}
}


#endif //MULTICAM_DATASET_UTILS_H
