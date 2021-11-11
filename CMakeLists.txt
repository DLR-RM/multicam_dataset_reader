#
# CMakeList.txt - top-level cmake configuration for project MulticamDatasetReader
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

cmake_minimum_required(VERSION 3.0)
message(STATUS " >> using cmake version ${CMAKE_VERSION}")

#
# setup cmake for a rmbuild configuration
#
message(STATUS "*************** pre init project *************** ")
include(cmake/MDRInitProject.cmake)

#
# setup main project
#
message(STATUS "*************** setup project *************** ")
project(MulticamDatasetReader)

#
# setup cmake for a rmbuild configuration
#
include(cmake/MDRSetupProject.cmake)

#
# pre-setup (correct order of targets w.r.t. dependencies)
#
message(STATUS "*************** sort tragets *************** ")
MDR_internal_sort_targets(${MDR_BUILD_TARGET_DIRS})

#
# setup build target projects
#
message(STATUS "*************** load targets *************** ")
foreach(__mdr_target ${MDR_BUILD_TARGET_DIRS_SORTED})
    add_subdirectory(${__mdr_target})
endforeach()

#
# add cmake config install rules
#
include(CMakePackageConfigHelpers)

set(CMAKE_CONFIG_DIR lib/MulticamDatasetReader/cmake)
set(INCLUDE_INSTALL_DIR include)
set(LIBRARY_INSTALL_DIR lib)
set(LIBRARY_INSTALL_DIR lib)
set(LIBRARIES "${MDR_LIB_TARGETS}")

# config file
configure_package_config_file(
    "cmake/templates/MulticamDatasetReaderConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MulticamDatasetReaderConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_CONFIG_DIR}"
    PATH_VARS CMAKE_CONFIG_DIR INCLUDE_INSTALL_DIR LIBRARY_INSTALL_DIR
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/MulticamDatasetReaderConfig.cmake"
    DESTINATION "${CMAKE_CONFIG_DIR}"
)

# version file
configure_file(
    "cmake/templates/MulticamDatasetReaderConfigVersion.cmake.in" "MulticamDatasetReaderConfigVersion.cmake" @ONLY
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/MulticamDatasetReaderConfigVersion.cmake"
    DESTINATION "${CMAKE_CONFIG_DIR}"
)

# install target properties
install(
    FILES "${MDR_CMAKE_TARGET_PROPERTIES_FILE}"
    DESTINATION "${CMAKE_CONFIG_DIR}"
)

# install find package files
# intall "cmake/<config>/MulticamDatasetReader_pkg_*.cmake"
file(GLOB MDR_CMAKE_TARGET_DEPENDENCY_FILES "${MDR_CONFIG_SETUP_DIR}/MulticamDatasetReader_pkg_*.cmake")
if(MDR_CMAKE_TARGET_DEPENDENCY_FILES)
    install(
        FILES ${MDR_CMAKE_TARGET_DEPENDENCY_FILES}
        DESTINATION "${CMAKE_CONFIG_DIR}"
    )
endif()

#
# end
#
