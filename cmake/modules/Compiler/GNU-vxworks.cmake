
#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)


# This module is shared by multiple languages; use include blocker.
if(__COMPILER_VXWORKS)
  return()
endif()
set(__COMPILER_VXWORKS 1)

include(Compiler/GNU)
include(Platform/vxworks)

macro(__compiler_vxworks lang)
    __compiler_gnu(${lang})
    set(CMAKE_SHARED_LIBRARY_${lang}_FLAGS "-fpic")

    foreach(type SHARED_LIBRARY SHARED_MODULE EXE)
        set(CMAKE_${type}_LINK_STATIC_${lang}_FLAGS "-static")
        set(CMAKE_${type}_LINK_DYNAMIC_${lang}_FLAGS "-non-static")
    endforeach()

    # Initial configuration flags.
    set(CMAKE_${lang}_FLAGS_INIT "")
    set(CMAKE_${lang}_FLAGS_DEBUG_INIT "-gdwarf-3")
    set(CMAKE_${lang}_FLAGS_MINSIZEREL_INIT "-Os -DNDEBUG")
    set(CMAKE_${lang}_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")
    set(CMAKE_${lang}_FLAGS_RELWITHDEBINFO_INIT "-O2 -gdwarf-3 -DNDEBUG")
endmacro()

__compiler_vxworks(C)
__compiler_vxworks(CXX)
