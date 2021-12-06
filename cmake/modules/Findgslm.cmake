#
# gslm
#

if(NOT gslm_VERSION)
    set(gslm_VERSION "latest")
endif()

set(gslm_ROOT_DIR "/home/laser-sc/packages/gslm/${gslm_VERSION}")
set(gslm_INCLUDE_DIR "${gslm_ROOT_DIR}/include")
set(gslm_INCLUDE_DIRS "${gslm_INCLUDE_DIR}")
set(gslm_LIBRARY_DIR "${gslm_ROOT_DIR}/lib/$ENV{DLRRM_HOST_PLATFORM}")
set(gslm_LIBRARY_DIRS "${gslm_LIBRARY_DIR}")

set(gslm_LIBS gslmnr gsl gslcblas)

foreach(_gslm_lib ${gslm_LIBS})
    unset(_tmp_gslm CACHE)
    find_library(_tmp_gslm ${_gslm_lib} PATHS ${gslm_LIBRARY_DIRS})
    list(APPEND gslm_LIBRARIES ${_tmp_gslm})
endforeach(_gslm_lib)

message(STATUS "gslm ${gslm_VERSION} from ${gslm_ROOT_DIR}")
mark_as_advanced(gslm_INCLUDE_DIR gslm_INCLUDE_DIRS gslm_LIBRARY_DIR gslm_LIBRARY_DIRS gslm_LIBRARIES)
