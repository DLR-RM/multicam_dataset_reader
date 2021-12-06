#
# DLR-RM rmbuild integration
#
# Copyright (c) 2017 Institute of Robotics and Mechatronics, DLR
#
#
#
# macros in this file:
# rmbuild_gen_cmake()
# Generates cmake files from rmbuild configuration.
#
#
#
# optional variables:
# RMBUILD_CMD - can be overloaded for using individual versions of 'rmbuild'
# RMBUILD_DEBUG  - turns on debug output
#
#
#

#
# default variables
#
set(RMBUILD_FOUND TRUE)
if(NOT RMBUILD_CMD)
    if($ENV{RMBUILD_CMD})
        message(STATUS "using rmbuild cmd from environment variable RMBUILD_CMD")
        set(RMBUILD_CMD $ENV{RMBUILD_CMD})
    else()
        find_program(RMBUILD_CMD
            NAMES rmbuild
        )
        if(NOT RMBUILD_CMD)
            message(WARNING "can't find rmbuild binary")
            set(RMBUILD_FOUND FALSE)
        endif()
    endif()
endif()

if(RMBUILD_FOUND)
    message(STATUS "using rmbuild from path ${RMBUILD_CMD}")

    if(NOT RMBUILD_DEBUG)
        set(RMBUILD_DEBUG OFF)
    endif()

    mark_as_advanced(
        RMBUILD_CMD
        RMBUILD_DEBUG
    )
endif()

#
# messages
#
function(rmbuild_status_message)
    message(STATUS "rmbuild: ${ARGN}")
endfunction(rmbuild_status_message)

function(rmbuild_error_message)
    message(FATAL_ERROR "rmbuild: ${ARGN}")
endfunction(rmbuild_error_message)

function(rmbuild_debug_message)
    if(RMBUILD_DEBUG MATCHES ON)
        message("rmbuild debug: ${ARGN}")
    endif()
endfunction(rmbuild_debug_message)

#
# rmbuild generate cmake info
#
function(rmbuild_gen_cmake_info project_dir)

    #call rmbuild
    rmbuild_status_message("exec: rmbuild cmake generate")
    execute_process(COMMAND "${RMBUILD_CMD}" "internal" "generate-cmake-info" "${project_dir}"
            OUTPUT_VARIABLE __RMBUILD_OUT
            ERROR_VARIABLE  __RMBUILD_ERR)

    #error check
    if(__RMBUILD_ERR)
        rmbuild_error_message("rmbuild returned with output:\n${__RMBUILD_OUT}\n and with error:\n${__RMBUILD_ERR}")
    endif()

    message(STATUS "[rmbuild out]: ${__RMBUILD_OUT}")
#    if(NOT __RMBUILD_OUT)
#        rmbuild_error_message("rmbuild returned with no output!\n")
#    endif()

    # cleanup
    unset(__RMBUILD_OUT)
    unset(__RMBUILD_ERR)

endfunction(rmbuild_gen_cmake_info)

#
# rmbuild generate cmake config
#
function(rmbuild_gen_cmake_config config project_dir build_dir build_type)

    #call rmbuild
    rmbuild_status_message("exec: rmbuild cmake generate")
    execute_process(COMMAND "${RMBUILD_CMD}" "internal" "generate-cmake-config" "--dbg" "${config}" "${project_dir}" "${build_dir}" "--build-type" "${build_type}" "--cmake-cmd" "${CMAKE_COMMAND}"
            OUTPUT_VARIABLE __RMBUILD_OUT
            ERROR_VARIABLE  __RMBUILD_ERR)

    #error check
    if(__RMBUILD_ERR)
        rmbuild_error_message("rmbuild returned with output:\n${__RMBUILD_OUT}\n and with error:\n${__RMBUILD_ERR}")
    endif()

    message(STATUS "[rmbuild out]: ${__RMBUILD_OUT}")
#    if(NOT __RMBUILD_OUT)
#        rmbuild_error_message("rmbuild returned with no output!\n")
#    endif()

    # cleanup
    unset(__RMBUILD_OUT)
    unset(__RMBUILD_ERR)

endfunction(rmbuild_gen_cmake_config)
