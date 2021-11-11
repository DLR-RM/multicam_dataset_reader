//
// Created by sewt_ma on 11/9/21.
//

#ifndef MDR_FILESYSTEM_H
#define MDR_FILESYSTEM_H

#include <Platform.config.h>

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

namespace MDR{

std::set<fs::path> get_sorted_directory_content(const fs::path& p);

}

#endif //MDR_FILESYSTEM_H