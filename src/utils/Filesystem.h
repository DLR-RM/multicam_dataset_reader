//
// Created by sewt_ma on 11/9/21.
//

#ifndef IGEL_SLAM_FILESYSTEM_H
#define IGEL_SLAM_FILESYSTEM_H

#include <Filesystem.config.h>

#ifdef HAS_STD_FILESYSTEM
#include <filesystem>
namespace fs=std::fs;

#else
#ifdef HAS_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace fs=boost::filesystem;

#else
#ifdef HAS_EXP_FILESYSTEM
#include <experimental/filesystem>
namespace fs=std::experimental::filesystem;
#endif
#endif
#endif

#include <set>

namespace IGEL{

std::set<fs::path> get_sorted_directory_content(const fs::path& p);

}

#endif //IGEL_SLAM_FILESYSTEM_H