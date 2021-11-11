#
# DLR-RM conan integration
#
# Copyright (c) 2018 Institute of Robotics and Mechatronics, DLR
#
#
#
# macros in this file:
# conan_install()
# Installs dependend packages to the local cache and generates cmake include files.
#
#
# optional variables:
# CONAN_CMD - can be overloaded for using individual versions of 'conan'
# CONAN_INSTALL_OPTIONS - additional conan install options
# CONAN_FILE - conafile to use (default: conanfile.py)
#

#
# default variables
#
if(NOT CONAN_CMD)
    set(CONAN_CMD "/opt/rmc-build-tools/any/cissy/bin/linux/conan")
endif()

mark_as_advanced(CONAN_CMD)

message(STATUS "using conan from path ${CONAN_CMD}")

#
# messages
#
function(conan_status_message)
    message(STATUS "conan: ${ARGN}")
endfunction(conan_status_message)

function(conan_error_message)
    message(FATAL_ERROR "conan: ${ARGN}")
endfunction(conan_error_message)

function(conan_debug_message)
    if(CONAN_DEBUG)
        message(STATUS "conan debug: ${ARGN}")
    endif()
endfunction(conan_debug_message)

#
# generate cmake files for find_package
#
function(conan_install)
    conan_debug_message("CONAN_NO_INSTALL ${CONAN_NO_INSTALL}")
    conan_debug_message("CONAN_FILE ${CONAN_FILE}")

    if(NOT CONAN_FILE)
        set(CONAN_FILE "conanfile.py")
    endif()

    if(CONAN_NO_INSTALL OR CONAN_IN_LOCAL_CACHE)
        conan_status_message("conan: skipping package install")
    else()
        #call conan
        set(CONAN_INSTALL_OPTIONS
                "install"
                ${CMAKE_CURRENT_SOURCE_DIR}/${CONAN_FILE}
                "--generator=cmake_paths"
                "--generator=cmake_smart_find_package"
                "--generator=pkg_config"
                "--build=outdated"
                "--profile=${RMBUILD_CONFIG}"
                "--update"
                ${CONAN_INSTALL_OPTIONS})

        conan_status_message("conan: starting package install")
        conan_status_message("(options: '${CONAN_INSTALL_OPTIONS}')")
        execute_process(COMMAND ${CONAN_CMD} ${CONAN_INSTALL_OPTIONS}
                OUTPUT_VARIABLE __CONAN_OUT
                ERROR_VARIABLE  __CONAN_ERR)

        # error check
        if(__CONAN_ERR)
            string(FIND "${__CONAN_ERR}" "ERROR:" matchres)
            if(NOT ${matchres} EQUAL -1)
                conan_error_message("conan returned with error:\n${__CONAN_ERR}")
            endif()
        endif()

        if(NOT __CONAN_OUT)
            conan_error_message("conan returned with no output (query not found?!?)\n")
        else()
            message(${__CONAN_OUT})
        endif()

        # cleanup
        unset(__CONAN_OUT)
        unset(__CONAN_ERR)
   endif()
endfunction(conan_install)