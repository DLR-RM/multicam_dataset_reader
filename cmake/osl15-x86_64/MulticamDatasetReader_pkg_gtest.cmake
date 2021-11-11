
if(NOT PkgConfig_FOUND)
    message(STATUS "> loading pkg-config support")
    find_package(PkgConfig REQUIRED)
endif()

if(NOT MulticamDatasetReader_gtest_LOADED)
    message(STATUS "> searching for pkgconfig package 'gtest'")
    pkg_check_modules(gtest REQUIRED IMPORTED_TARGET gtest)
    set(MulticamDatasetReader_gtest_LOADED True)
endif()
    