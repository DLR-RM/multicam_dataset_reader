#########################################
# Multicam Dataset
# DLR-RMC 2021
# Marco Sewtz
#
# @Description
#
# @Disclaimer
#########################################

# TODO check cmake versions
cmake_minimum_required(VERSION 3.10)

project(multicam_dataset
        VERSION 0.1.0
        LANGUAGES CXX
        )

# build type
set(default_build_type "RelWithDebInfo")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# position independent code
option(USE_fPIC "Compile with Position Independent Code (fPIC) flag" ON)
set(CMAKE_POSITION_INDEPENDENT_CODE USE_fPIC)
message(STATUS "fPIC is set to ${USE_fPIC}")

# Boost
option(USE_BOOST "Use Boost instead of std filesystem" ON)
if(USE_BOOST)
    option(Boost_USE_STATIC_LIBS "Use Boost static libs" OFF)
    option(Boost_USE_MULTITHREADED "Use multithreaded Boost funtions" ON)
    option(Boost_USE_STATIC_RUNTIME "Use static runtime Boost" OFF)

    find_package(Boost 1.58 REQUIRED COMPONENTS filesystem)
    add_definitions("-DMD_USE_BOOST_FILESYSTEM")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/src/dataset_reader")

# enable testing
option(BUILD_TESTS "Build tests" ON)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTS)
    add_subdirectory("${PROJECT_SOURCE_DIR}/test")
endif()

# install
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        MulticamDatasetVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS multicam_dataset
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )