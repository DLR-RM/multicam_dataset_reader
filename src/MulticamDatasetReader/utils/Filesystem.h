//
// Created by sewt_ma on 11/9/21.
//

#ifndef MDR_FILESYSTEM_H
#define MDR_FILESYSTEM_H


#include <boost/filesystem.hpp>
namespace fs=boost::filesystem;

#include <set>
#include <vector>

namespace MDR{

/**
 * Get a sorted set of all directory items
 * \param p path to directory
 * \return set of items
 */
std::set<fs::path> get_sorted_directory_content(const fs::path& p);

/**
 * Get all directories
 * \param p path to directory
 * \return vector of directories
 */
std::vector<fs::path> get_dirs(const fs::path& p);

/**
 * Get all files
 * \param p path to directory
 * \return vector of files
 */
std::vector<fs::path> get_files(const fs::path& p);

}

#endif //MDR_FILESYSTEM_H