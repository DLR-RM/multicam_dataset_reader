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


option(USE_fPIC "Compile with Position Independent Code (fPIC) flag" ON)
option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_TESTS "Build tests" ON)

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
set(CMAKE_POSITION_INDEPENDENT_CODE USE_fPIC)
message(STATUS "fPIC is set to ${USE_fPIC}")

# Boost
option(USE_BOOST "Use Boost instead of std filesystem" ON)
if(USE_BOOST)
    option(Boost_USE_STATIC_LIBS "Use Boost static libs" OFF)
    option(Boost_USE_MULTITHREADED "Use multithreaded Boost funtions" ON)
    option(Boost_USE_STATIC_RUNTIME "Use static runtime Boost" OFF)

    if(BUILD_TESTS)
        find_package(Boost 1.58 REQUIRED COMPONENTS unit_test_framework filesystem system)
    else()
        find_package(Boost 1.58 REQUIRED COMPONENTS filesystem)
    endif()
    add_definitions("-DMD_USE_BOOST_FILESYSTEM")
endif()

# add reader target
set(READER_ROOT "src/dataset_reader")
configure_file(${READER_ROOT}/Version.h.in ${PROJECT_BINARY_DIR}/${READER_ROOT}/Version.h)

set(LIB_HEADERS
        ${READER_ROOT}/DatasetReader.h
        ${READER_ROOT}/Filesystem.h
        ${READER_ROOT}/Utils.h
        ${READER_ROOT}/Version.h
        )

set(LIB_SOURCES
        ${READER_ROOT}/DatasetReader.cc
        ${READER_ROOT}/Utils.cc
        )

add_library(multicam_dataset_reader ${LIB_SOURCES} ${LIB_HEADERS})
set_target_properties(multicam_dataset_reader PROPERTIES PUBLIC_HEADER "${LIB_HEADERS}")

# enable examples
if(BUILD_EXAMPLES)
    include_directories("${PROJECT_SOURCE_DIR}/src")

    add_executable(read_dataset ${PROJECT_SOURCE_DIR}/examples/read_dataset.cc)
    target_link_libraries(read_dataset ${Boost_LIBRARIES} multicam_dataset_reader)
endif()

# enable testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTS)
    set(TEST_ROOT "test")
    add_executable(test_reader ${TEST_ROOT}/test_reader.cc)
    include_directories("${PROJECT_SOURCE_DIR}/src")
    target_link_libraries(test_reader ${Boost_LIBRARIES} multicam_dataset_reader)
    add_test(tests test_reader)
endif()

# install
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        MulticamDatasetVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS multicam_dataset_reader read_dataset
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        INCLUDES DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )