#
# MDR Modules
#

#
# Utils submodule
#
if(NOT OpenCV_core_FOUND)
    find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)
endif()

if(NOT Boost_filesystem_FOUND)
    find_package(Boost REQUIRED COMPONENTS "filesystem")
endif()

if (NOT EIGEN_FOUND)
    find_package(Eigen REQUIRED)
endif()

add_library_target(utils
        STATIC
        SOURCE_PATTERN "utils/*.cc"
        HEADER_PATTERN "utils/*.h"
        INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${EIGEN_INCLUDE_DIRS}
        LIBRARIES ${OpenCV_LIBS} ${Boost_LIBRARIES}
        )

add_library_target(core
        SHARED
        SOURCE_PATTERN "core/*.cc"
        HEADER_PATTERN "core/*.h"
        DEPENDENCIES utils
        INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS}
        LIBRARIES ${OpenCV_LIBS}
        )