#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# testing macro
#
macro(MDR_add_test)
    #
    # check for initialized project (lib or exe)
    #
    if (NOT DEFINED ${PROJECT_NAME}_FOUND)
        message(FATAL_ERROR "can't add test, project not specified!")
    endif ()

    #
    # add test if project is supported on current target platform
    #
    if (${${PROJECT_NAME}_SUPPORTED})
        # parse args
        set(oneValueArgs NAME COMMAND SETUP_COMMAND SHUTDOWN_COMMAND WORKING_DIRECTORY)
        set(multiValueArgs ARGS ENVIRONMENT)
        cmake_parse_arguments(${PROJECT_NAME}_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        # debug output
        debug_message("NAME ${${PROJECT_NAME}_TEST_NAME}")
        debug_message("COMMAND ${${PROJECT_NAME}_TEST_COMMAND}")
        debug_message("SETUP ${${PROJECT_NAME}_TEST_SETUP_COMMAND}")
        debug_message("SHUTDOWN ${${PROJECT_NAME}_TEST_SHUTDOWN_COMMAND}")
        debug_message("ARGS ${${PROJECT_NAME}_TEST_ARGS}")
        debug_message("WORKING_DIRECTORY ${${PROJECT_NAME}_TEST_WORKING_DIRECTORY}")
        debug_message("ENVIRONMENT ${${PROJECT_NAME}_TEST_ENVIRONMENT}")

        # check required variables: NAME, COMMAND
        if(NOT ${PROJECT_NAME}_TEST_NAME)
            message(FATAL_ERROR "specified test without NAME")
        endif()

        if(NOT ${PROJECT_NAME}_TEST_COMMAND)
            message(FATAL_ERROR "specified test without COMMAND")
        endif()

        # check workspace dir, use project build directory as default
        if(NOT ${PROJECT_NAME}_TEST_WORKING_DIRECTORY)
            set(${PROJECT_NAME}_TEST_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
        endif()

        # generate environment
        set(_env "")
        foreach (_entry ${${PROJECT_NAME}_TEST_ENVIRONMENT})
            set(_env "export ${_entry}\n${_env}")
        endforeach ()

        # lookup path to command
        if (${${PROJECT_NAME}_TEST_COMMAND} STREQUAL ${${PROJECT_NAME}_TARGET_NAME})
            set(${PROJECT_NAME}_TEST_COMMAND_ABS ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_TEST_COMMAND})
        else ()
            find_program(${PROJECT_NAME}_TEST_COMMAND_ABS
                    ${${PROJECT_NAME}_TEST_COMMAND}
                    PATH ${CMAKE_CURRENT_BINARY_DIR})
        endif ()

        # generate wrapper script
        set(TEST_WRAPPER_LD_PATH $ENV{LD_LIBRARY_PATH})
        set(TEST_WRAPPER_WORKSPACE_DIR ${${PROJECT_NAME}_TEST_WORKING_DIRECTORY})
        set(TEST_WRAPPER_EXECUTABLE ${${PROJECT_NAME}_TEST_COMMAND_ABS})
        set(TEST_WRAPPER_ARGS ${${PROJECT_NAME}_TEST_ARGS})
        set(TEST_WRAPPER_SETUP_COMMAND ${${PROJECT_NAME}_TEST_SETUP_COMMAND})
        set(TEST_WRAPPER_SHUTDOWN_COMMAND ${${PROJECT_NAME}_TEST_SHUTDOWN_COMMAND})
        set(TEST_WRAPPER_ADDITIONAL_ENV ${_env})

        configure_file(
                ${MDR_CMAKE_DIR}/templates/ExecTest.sh.in
                ${CMAKE_CURRENT_BINARY_DIR}/ExecTest.sh
                @ONLY)

        #add test
        add_test(NAME ${${PROJECT_NAME}_TEST_NAME} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/ExecTest.sh)
    endif ()

endmacro(MDR_add_test)


#
# dumps all variables that match a pattern
# e.g. dump_cmake_variables("yaml") will list all vars containing "yaml".
#
function(MDR_dump_cmake_variables)
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
endfunction(MDR_dump_cmake_variables)