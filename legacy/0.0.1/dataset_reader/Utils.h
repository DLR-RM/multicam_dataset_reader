//
// Created by sewt_ma on 7/28/21.
//

#ifndef MULTICAM_DATASET_UTILS_H
#define MULTICAM_DATASET_UTILS_H

#include "Filesystem.h"

#include <Eigen/Eigen>

#include <sstream>
#include <vector>
#include <string>

namespace MD::utils{

///////////////////////////////////////////////////////////////////////////
// Filesystem
///////////////////////////////////////////////////////////////////////////

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

/**
 * enum class for file types
 */
enum class fs_type{
	dir,
	file,
	other
};

/**
 * check that all given files of specific type exists
 * \param root folder to check
 * \param items list of names to check
 * \param type type to check
 * \return True if all were found
 */
bool check_for(const fs::path& root,
			   const std::vector<std::string>& items,
			   fs_type type);

/**
 * Get type of file
 * \param path path to file
 * \return file type
 */
fs_type get_type(const fs::path& path);

/**
 * List all files of specific type in directory
 * \param path directory
 * \param type type
 * \return vector containing all files
 */
std::vector<std::string> get_all_of_type(const fs::path& path, fs_type type);

///////////////////////////////////////////////////////////////////////////
// Strings
///////////////////////////////////////////////////////////////////////////

/**
 * Check if string starts with test pattern
 * \param str string to test
 * \param test pattern to find
 * \return True if found
 */
bool str_starts_with(const std::string& str, const std::string& test);

/**
 * Check if string contains nothing or only whitespaces
 * \param str string to test
 * \return True if empty or white
 */
bool str_empty_or_white(const std::string& str);

/**
 * Split a string
 * \param str string to split
 * \param delimiter delimiter string
 * \return vector without delimiter
 */
template<typename T>
std::vector<T> str_split(const std::string& str, char delimiter){
	std::vector<T> elements;
	std::istringstream ss(str);

	for(std::string line; std::getline(ss, line, delimiter); ){
		T tmp;
		std::istringstream(line) >> tmp;
		elements.emplace_back(tmp);
	}

	return elements;
}

///////////////////////////////////////////////////////////////////////////
// Math / Matrix
///////////////////////////////////////////////////////////////////////////

/**
 * Convert quaternion representation to matrix representation
 * \param quats
 * \return 3x3 rot matrix
 */
Eigen::Matrix3d mat_quat2pose(const Eigen::Quaterniond& quats);

/**
 * Convert quaternion representation to matrix representation
 * \param quats_vec vector containing representation
 * \return 3x3 rot matrix
 */
Eigen::Matrix3d mat_quat2pose(std::vector<double>& quats_vec);

/**
 * Get full pose matrix from single values with quaternions
 * \param elements_vec 7 element vector
 * \return 4x4 pose amtrix
 */
Eigen::Matrix4d mat_get_posemat(std::vector<double>& elements_vec);

}


#endif //MULTICAM_DATASET_UTILS_H
