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

# conan integration
if(EXISTS "${CMAKE_SOURCE_DIR}/conanfile.py")
    message(STATUS "This seems to be a conan repository")
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake OR NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

        message(STATUS "conan files not found ... generating")
        set(CONAN_CMD
                "conan"
                "install"
                ${CMAKE_SOURCE_DIR}/${CONAN_FILE}
                "--generator=cmake_paths"
                "--generator=cmake"
                "--profile=$ENV{DLRRM_HOST_PLATFORM}-gcc8"
                "--update"
                "-b" "missing")

        execute_process(COMMAND ${CONAN_CMD}
                OUTPUT_VARIABLE __CONAN_OUT
                ERROR_VARIABLE  __CONAN_ERR)

        # error check
        if(__CONAN_ERR)
            string(FIND "${__CONAN_ERR}" "ERROR:" matchres)
            if(NOT ${matchres} EQUAL -1)
                message(ERROR "conan returned with error:\n${__CONAN_ERR}")
            endif()
        endif()

        if(NOT __CONAN_OUT)
            message(ERROR "conan returned with no output (query not found?!?)\n")
        else()
            message(${__CONAN_OUT})
        endif()

        # cleanup
        unset(__CONAN_OUT)
        unset(__CONAN_ERR)
    endif()

    #include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    #conan_basic_setup(TARGETS)

    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_fPIC "Compile with Position Independent Code (fPIC) flag" ON)
option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_TESTS "Build tests" ON)
option(USE_BOOST "Use Boost instead of std filesystem" OFF)

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

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

# OpenCV
find_package(OpenCV REQUIRED core imgcodecs highgui)
find_package(gdal REQUIRED)
link_directories(${gdal_LIBRARY_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

# Boost
if(USE_BOOST)
    option(Boost_USE_STATIC_LIBS "Use Boost static libs" OFF)
    option(Boost_USE_MULTITHREADED "Use multithreaded Boost funtions" ON)
    option(Boost_USE_STATIC_RUNTIME "Use static runtime Boost" OFF)

    find_package(Boost 1.58 REQUIRED COMPONENTS filesystem)
    add_definitions("-DMD_USE_BOOST_FILESYSTEM")
else()
    # TODO: Check if supporting filesystem
endif()

# add reader target
set(TARGETS ${TARGETS} multicam_dataset_reader)
set(READER_ROOT "src/dataset_reader")
configure_file(${READER_ROOT}/Version.h.in ${PROJECT_BINARY_DIR}/${READER_ROOT}/Version.h)

set(LIB_HEADERS
        ${READER_ROOT}/TimedMapIterator.h
        ${READER_ROOT}/DatasetReader.h
        ${READER_ROOT}/Filesystem.h
        ${READER_ROOT}/Logging.h
        ${READER_ROOT}/Sensor.h
        ${READER_ROOT}/Utils.h
        ${READER_ROOT}/Data.h
        ${READER_ROOT}/Version.h
        )

set(LIB_SOURCES
        ${READER_ROOT}/TimedMapIterator.cc
        ${READER_ROOT}/DatasetReader.cc
        ${READER_ROOT}/Logging.cc
        ${READER_ROOT}/Sensor.cc
        ${READER_ROOT}/Utils.cc
        ${READER_ROOT}/Data.cc
        )

add_library(multicam_dataset_reader ${LIB_SOURCES} ${LIB_HEADERS})
target_link_libraries(multicam_dataset_reader ${OpenCV_LIBS} ${gdal_LIBS} stdc++fs)
set_target_properties(multicam_dataset_reader PROPERTIES PUBLIC_HEADER "${LIB_HEADERS}")

# enable examples
if(BUILD_EXAMPLES)
    include_directories("${PROJECT_SOURCE_DIR}/src")

    set(TARGETS ${TARGETS} read_dataset)
    add_executable(read_dataset ${PROJECT_SOURCE_DIR}/examples/read_dataset.cc)
    target_link_libraries(read_dataset multicam_dataset_reader ${OpenCV_LIBS} ${gdal_LIBS} stdc++fs)
endif()

# install
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        MulticamDatasetVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion
)

message(STATUS "CXXFLAGS " ${CMAKE_CXX_FLAGS})
install(TARGETS ${TARGETS}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include
        INCLUDES DESTINATION include
        FILES_MATCHING PATTERN "*.h"
        )

function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        if (ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
            if (NOT MATCHED)
                continue()
            endif()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()