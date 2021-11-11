#
# MDRSetupProject.cmake - common cmake setup and platform selection for project MulticamDatasetReader
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# load rmpm support
#
find_package(rmpm REQUIRED)

#
# load conan support
#
find_package(conan REQUIRED)

#
# enable testing (generation ctest configs)
#
enable_testing()

#
# some global options
#
set(MDR_BUILD_SHARED_LIBRARIES OFF CACHE BOOL "build shared libraries")

#
# install section
#

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_INCLUDE_STRIP_PATH ${MDR_ROOT_DIR} CACHE PATH "Path to be stripped from include file path")

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
# global build variables
#
set(MDR_INCLUDE_DIR ${MDR_ROOT_DIR})
set(MDR_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(MDR_CMAKE_TARGET_PROPERTIES_FILE "${CMAKE_CURRENT_BINARY_DIR}/MulticamDatasetReaderTargetProperties.cmake")
file(REMOVE "${MDR_CMAKE_TARGET_PROPERTIES_FILE}")
file(WRITE "${MDR_CMAKE_TARGET_PROPERTIES_FILE}" "# automatically generated properties for MulticamDatasetReader \n\n")

#
# misc defaults
#
set(MDR_DEFAULT_HEADER_PATTERN "*.h" "*.hpp" "*.h" "*.icc" "*.hcc")
set(MDR_DEFAULT_SOURCE_PATTERN "*.c" "*.cc" "*.cpp")

#
# load macros
#
include(cmake/MDRMacros.cmake)

#
# load configuration specific setup (or default config if nothing specific found)
#

set(MDR_DEFAULT_SETUP_DIR "${MDR_CMAKE_DIR}/default")
set(MDR_CONFIG_SETUP_DIR "${MDR_CMAKE_DIR}/${RMBUILD_CONFIG}")

if(EXISTS ${MDR_CONFIG_SETUP_DIR}/MDRSetupConfiguration.cmake)
    message(STATUS "loading rmbuild config for ${RMBUILD_CONFIG}")
    include(${MDR_CONFIG_SETUP_DIR}/MDRSetupConfiguration.cmake)

elseif(EXISTS ${MDR_DEFAULT_SETUP_DIR}/MDRSetupConfiguration.cmake)
    message(STATUS "loading rmbuild default config (no specialized config for ${RMBUILD_CONFIG} found)")
    include(${MDR_DEFAULT_SETUP_DIR}/MDRSetupConfiguration.cmake)

else()
    message(FATAL_ERROR "rmbuild config setup '${MDR_CONFIG_SETUP_DIR}/MDRSetupConfiguration.cmake' not found!")

endif()

#
# mark as loaded
#
set(MDR_SETUP_DONE TRUE CACHE INTERNAL "Xxx basic setup")
