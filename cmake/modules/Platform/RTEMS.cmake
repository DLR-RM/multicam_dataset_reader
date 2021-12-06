#
# rtems common system definitions for cmake
#

# This module is shared by multiple languages; use include blocker.
if(__PLATFORM_RTEMS)
  return()
endif()
set(__PLATFORM_RTEMS 1)

# >>>>> extracted from https://raw.githubusercontent.com/nasa/PSP/master/cmake/Modules/Platform/RTEMS.cmake

# Note - RTEMS does not support "shared libs" in the traditional sense,
# but it does have a loader that does static linking at runtime.
# This property is set true which allows one to use the CMake shared library logic
# But the code is otherwise built as static -- no PIC flags
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".obj")
set(CMAKE_SHARED_MODULE_SUFFIX ".obj")

# Setting all these to empty string defeats the default behavior
# of adding an -fPIC option to shared library/module code.
set(CMAKE_DL_LIBS "")
set(CMAKE_C_COMPILE_OPTIONS_PIC "")
set(CMAKE_CXX_COMPILE_OPTIONS_PIC "")



# -----------------------------------------------------------------
#  CMake Module for RTEMS
#
# The cmake distribution by default does not contain a module for RTEMS
# -----------------------------------------------------------------

# Note - RTEMS does not support "shared libs" in the traditional sense,
# but it does have a loader that does static linking at runtime.
# This property is set true which allows one to use the CMake shared library logic
# But the code is otherwise built as static -- no PIC flags
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".obj")
set(CMAKE_SHARED_MODULE_SUFFIX ".obj")

# Setting all these to empty string defeats the default behavior
# of adding an -fPIC option to shared library/module code.
set(CMAKE_DL_LIBS "")
set(CMAKE_C_COMPILE_OPTIONS_PIC "")
set(CMAKE_CXX_COMPILE_OPTIONS_PIC "")

set(CMAKE_SHARED_LIBRARY_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "")
set(CMAKE_SHARED_MODULE_C_FLAGS "")
set(CMAKE_SHARED_MODULE_CXX_FLAGS "")

set(RTEMS TRUE)
set(UNIX TRUE)
