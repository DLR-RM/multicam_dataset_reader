//
// Created by sewt_ma on 2/17/21.
//

#ifdef MD_USE_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif