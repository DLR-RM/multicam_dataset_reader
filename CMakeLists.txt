##################################################
# CMake Project for MDR Build
# Marco Sewtz
# DLR, RMC - 2022
##################################################

cmake_minimum_required(VERSION 3.12)
message(STATUS " >> using cmake version ${CMAKE_VERSION}")

project("multicam_dataset_reader")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#
# options
#
option(BUILD_EXAMPLES "Build examples" ON)

#
# includes
#
find_package(PkgConfig REQUIRED)

#
# set root path and module path
#
get_filename_component(MDR_ROOT_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(MDR_SOURCE_DIR "${MDR_ROOT_DIR}/src" ABSOLUTE)
get_filename_component(MDR_EXAMPLE_DIR "${MDR_ROOT_DIR}/examples" ABSOLUTE)
set(MDR_CMAKE_DIR "${MDR_ROOT_DIR}/cmake")
set(MDR_MODULE_DIR "${MDR_CMAKE_DIR}/modules")
list(APPEND CMAKE_MODULE_PATH "${MDR_MODULE_DIR}" $ENV{CMAKE_MODULE_PATH})
get_filename_component(MDR_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} ABSOLUTE)

#
# look if this is a conan build
# this will load the "conan_paths.cmake" file into the current
# cmake environment
#
if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Loading " ${CMAKE_TOOLCHAIN_FILE})
    include(${CMAKE_TOOLCHAIN_FILE})
else()
    file(GLOB_RECURSE CONAN_PATHS_FILE "*/conan_paths.cmake")
    if(CONAN_PATHS_FILE)
        message(STATUS "Conan paths file at ${CONAN_PATHS_FILE}")
        include(${CONAN_PATHS_FILE})
    else()
        message(STATUS "No conan paths file detected")
    endif()
endif()

#
# Offer the user the choice of overriding the installation directories
#
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

#
# setup version
#
if(NOT DEFINED MDR_VERSION_STRING)
    set(MDR_MAJOR_VERSION 0)
    set(MDR_MINOR_VERSION 0)
    set(MDR_PATCH_VERSION 0)
    set(MDR_VERSION ${MDR_MAJOR_VERSION}.${MDR_MINOR_VERSION}.${MDR_PATCH_VERSION})
    message(STATUS "setup default version ${MDR_VERSION}")
else()
    string(REPLACE "." ";" VERSION_LIST ${MDR_VERSION_STRING})

    list(GET VERSION_LIST 0 MDR_MAJOR_VERSION)
    list(GET VERSION_LIST 1 MDR_MINOR_VERSION)
    list(GET VERSION_LIST 2 MDR_PATCH_VERSION)
    set(MDR_VERSION ${MDR_MAJOR_VERSION}.${MDR_MINOR_VERSION}.${MDR_PATCH_VERSION})
    message(STATUS "setup user version ${MDR_VERSION}")
endif()

#
# set cmake build type to "RelWithDebInfo" if not otherwise specified
#
if(NOT CMAKE_BUILD_TYPE)
    #valid values: Debug, Release, RelWithDebInfo and MinSizeRel.
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()
message(STATUS "Build type is " ${CMAKE_BUILD_TYPE})

#
# include macros
#
include(cmake/macros.cmake)

#
# handle modules
#
include_directories(${MDR_SOURCE_DIR})
add_subdirectory(${MDR_SOURCE_DIR}/MulticamDatasetReader)

include_directories(${MDR_EXAMPLE_DIR})
if(${BUILD_EXAMPLES})
    add_subdirectory(${MDR_EXAMPLE_DIR})
endif()