#
# MDRInitProject.txt - initial cmake configuration for project MulticamDatasetReader - has to be loaded before 'project' function
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# basic setup
#
set(CMAKE_COLOR_MAKEFILE ON)

#
# set root path and module path
#
get_filename_component(MDR_ROOT_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(MDR_ROOT_DIR "${MDR_ROOT_DIR}" PATH) #go one directory level back
set(MDR_CMAKE_DIR "${MDR_ROOT_DIR}/cmake")
set(MDR_MODULE_DIR "${MDR_CMAKE_DIR}/modules")
list(APPEND CMAKE_MODULE_PATH "${MDR_MODULE_DIR}")
get_filename_component(MDR_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} ABSOLUTE)

#
# set cmake build type to "RelWithDebInfo" if not otherwise specified
#
if(NOT CMAKE_BUILD_TYPE)
#valid values: Debug, Release, RelWithDebInfo and MinSizeRel.
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

#
# try to load rmbuild
#
find_package(rmbuild)

#
# (re-)generate rmbuild generated cmake files
# (this updates the build targets and
#  the configuration specific settings)
#
if(RMBUILD_FOUND)
    message(STATUS "updating rmbuild cmake info files")
    rmbuild_gen_cmake_info(${MDR_ROOT_DIR})
else()
    message(WARNING "updating rmbuild cmake info files ... skipped (rmbuild not found)")
endif()

#
# load rmbuild generated project info
#
if(EXISTS "${MDR_CMAKE_DIR}/MDRProjectInfo.cmake")
    include(${MDR_CMAKE_DIR}/MDRProjectInfo.cmake)
else()
    message(FATAL_ERROR "can't load 'cmake/MDRProjectInfo.cmake' ... run rmbuild manually? ")
endif()

#
# determine rmbuild configuration from environment and generate rmbuild cmake config
# (should only happen if rmbuild config doesn't exist)
#
if(NOT RMBUILD_CONFIG)
    set(_ARCHS ${RMBUILD_PLATFORM} $ENV{DLRRM_HOST_PLATFORM} $ENV{OS} $ENV{OSTYPE})
    foreach(_ARCH ${_ARCHS})
        #message(STATUS "testing for MDR_CONFIGURATIONS_${_ARCH} ${MDR_CONFIGURATIONS_${_ARCH}}")
        if(MDR_CONFIGURATIONS_${_ARCH})
            #message(STATUS "testing for MDR_CONFIGURATIONS_${_ARCH} ... okay")
            list(GET MDR_CONFIGURATIONS_${_ARCH} 0 RMBUILD_CONFIG)
            set(RMBUILD_PLATFORM ${_ARCH})
            break()
        endif()
    endforeach()

    if(NOT RMBUILD_CONFIG)
        message(FATAL_ERROR "can't determine build configuration for platform strings ''${_ARCHS}' ... platform not supported?")
    endif()

    message(STATUS "using auto-detected build configuration ${RMBUILD_CONFIG}")
    #message(STATUS "using corresponding build platform ${RMBUILD_PLATFORM}")
    unset(_ARCHS)


else()
    message(STATUS "using rmbuild configuration ${RMBUILD_CONFIG}")
    #message(STATUS "using rmbuild platform ${RMBUILD_PLATFORM}")

endif()

#
# generate and load rmbuild build config files (platform & config info)
#
if(RMBUILD_FOUND)
    message(STATUS "updating rmbuild cmake config files ...")
    rmbuild_gen_cmake_config(${RMBUILD_CONFIG} ${MDR_ROOT_DIR} ${MDR_BUILD_DIR} ${CMAKE_BUILD_TYPE})
else()
    message(WARNING "updating rmbuild cmake config files ...  skipped (rmbuild not found)")
endif()

#
# load rmbuild platform info file
# (contains compiler and tool settings)
#
set(RMBUILD_PLATFORM_INFO_FILE ${MDR_CMAKE_DIR}/${RMBUILD_CONFIG}/MDRPlatformInfo.cmake)
if(NOT EXISTS ${RMBUILD_PLATFORM_INFO_FILE})
    message(FATAL_ERROR "missing rmbuild file: ${RMBUILD_PLATFORM_INFO_FILE}")
endif()

message(STATUS "loading rmbuild platform info ${RMBUILD_PLATFORM_INFO_FILE}")
include(${RMBUILD_PLATFORM_INFO_FILE})

set(RMBUILD_PLATFORM_INFO_FILE_LOADED TRUE)

#
# generate target file for rmbuild
#
set(MDR_TARGET_FILE ${MDR_BUILD_DIR}/targets.yaml)
file(WRITE ${MDR_TARGET_FILE} "#rmbuild targets (automatically generated... do not edit!\n")