#
# Examples
#

pkg_check_modules(tclap REQUIRED tclap)
if(NOT OpenCV_core_FOUND)
    find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)
endif()

if(NOT Boost_filesystem_FOUND)
    find_package(Boost REQUIRED COMPONENTS "system" "filesystem")
endif()

if (NOT EIGEN_FOUND)
    find_package(Eigen REQUIRED)
endif()

add_executable_target(example_load_dataset
        SOURCE_PATTERN "load_dataset/*.cc"
        DEPENDENCIES utils core
        INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${tclap_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS}
        LIBRARIES ${OpenCV_LIBS} ${Boost_LIBRARIES}
        )