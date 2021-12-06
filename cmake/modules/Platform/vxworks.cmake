#
# vxworks common system definitions for cmake
#

# This module is shared by multiple languages; use include blocker.
if(__PLATFORM_VXWORKS)
  return()
endif()
set(__PLATFORM_VXWORKS 1)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

#
# vxworks paths and defines
#
set(VXWORKS_VSB_DIR "/volume/USERSTORE/stel_ma/foreign_packages/vxworks/workspace/vsb_SANDYBRIDGE_64_smp")

set(VXWORKS_DEFINES
    -DCPU=_VX_SANDYBRIDGE
    -DTOOL_FAMILY=gnu
    -DTOOL=gnu
    -D_VSB_CONFIG_FILE="${VXWORKS_VSB_DIR}/h/config/vsbConfig.h"
    -DTIMTEST
)

set(VXWORKS_INCLUDE_DIRS
    "${VXWORKS_VSB_DIR}/share/h"
    "${VXWORKS_VSB_DIR}/usr/h/public"
    "${VXWORKS_VSB_DIR}/usr/h"
)

set(VXWORKS_LIBRARY_DIRS
   ${VXWORKS_VSB_DIR}/usr/lib/common/PIC
   ${VXWORKS_VSB_DIR}/usr/lib/common
)


# Shared libraries with no builtin soname may not be linked safely by
# specifying the file path.
set(CMAKE_PLATFORM_USES_PATH_WHEN_NO_SONAME 1)

# Initialize C link type selection flags.  These flags are used when
# building a shared library, shared module, or executable that links
# to other libraries to select whether to use the static or shared
# versions of the libraries.
#foreach(type SHARED_LIBRARY SHARED_MODULE EXE)
#    set(CMAKE_${type}_LINK_STATIC_C_FLAGS "-Wl,-Bstatic")
#    set(CMAKE_${type}_LINK_DYNAMIC_C_FLAGS "-Wl,-Bdynamic")
#endforeach()

#
# add system includes and defines
#
include_directories(SYSTEM ${VXWORKS_INCLUDE_DIRS})
add_definitions(${VXWORKS_DEFINES})
link_directories(${VXWORKS_LIBRARY_DIRS})

#
# force compiler (even if the test fail)
#
include(CMakeForceCompiler)
CMAKE_FORCE_C_COMPILER(${CMAKE_C_COMPILER} GNU-vxworks)
CMAKE_FORCE_CXX_COMPILER(${CMAKE_CXX_COMPILER} GNU-vxworks)
