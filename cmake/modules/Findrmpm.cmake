#
# DLR-RM rmpm integration
#
# Copyright (c) 2017 Institute of Robotics and Mechatronics, DLR
#
#
#
# macros in this file:
# rmpm_env(PACKAGES package1 package2 ...
#          OPTIONS option1 option2 ...
#          FILE filename)
# Generates an environment from a set of packages. Additional rmpm_do options can be specified.
# If FILE option is set the found environment is stored as bash script.
#
# rmpm_key(KEY key
#          PACKAGE package
#          OUTPUT output
#          OPTIONS option1 option2 ...)
# Reads a key from package and writes it to output. Additional rmpm_do options can be specified.
#
#
# optional variables:
# RMPM_DO_CMD - can be overloaded for using individual versions of 'rmpm_do'
# RMPM_DEBUG  - turns on debug output
#
#
#

#needed for arg parsing
include(CMakeParseArguments)

#
# default variables
#
if(NOT RMPM_DO_CMD)
    set(RMPM_DO_CMD "/volume/software/common/bin/$ENV{DLRRM_HOST_PLATFORM}/rmpm_do")
endif()

if(NOT RMPM_DEBUG)
    set(RMPM_DEBUG OFF)
endif()

mark_as_advanced(
    RMPM_DO_CMD
    RMPM_DEBUG
)

message(STATUS "using rmpm from path ${RMPM_DO_CMD}")

#
# messages
#
function(rmpm_status_message)
    message(STATUS "rmpm: ${ARGN}")
endfunction(rmpm_status_message)

function(rmpm_error_message)
    message(FATAL_ERROR "rmpm: ${ARGN}")
endfunction(rmpm_error_message)

function(rmpm_debug_message)
    if(RMPM_DEBUG MATCHES ON)
        message("rmpm debug: ${ARGN}")
    endif()
endfunction(rmpm_debug_message)

#
# rmpm_gen_temp_name
# generates a unique temporary name
#
macro(rmpm_gen_temp_name fname)
    if(${ARGC} GREATER 1) # Have to escape ARGC to correctly compare
        set(_base ${ARGV1})
    else(${ARGC} GREATER 1)
        set(_base ".cmake-tmp")
    endif(${ARGC} GREATER 1)
    set(_counter 0)
    while(EXISTS "${_base}${_counter}")
        math(EXPR _counter "${_counter} + 1")
    endwhile(EXISTS "${_base}${_counter}")
    set(${fname} "${_base}${_counter}")
endmacro(rmpm_gen_temp_name)

# Evaluate expression
# Suggestion from the Wiki: http://cmake.org/Wiki/CMake/Language_Syntax
# Unfortunately, no built-in stuff for this: http://public.kitware.com/Bug/view.php?id=4034
macro(rmpm_eval_cmd expr)
    rmpm_gen_temp_name(_fname)
    file(WRITE ${_fname} "${expr}")
    include(${_fname})
    file(REMOVE ${_fname})
endmacro(rmpm_eval_cmd)

#
# rmpm_do query
# (query packages and store them into a query db file)
#
function(rmpm_query ...)
    #parse args
    set(options "")
    set(oneValueArgs QUERYFILE)
    set(multiValueArgs PACKAGES OPTIONS)
    cmake_parse_arguments(RMPM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGV})

    #call rmpm
    rmpm_status_message("rmpm env with ${ARGV}")
    execute_process(COMMAND "${RMPM_DO_CMD}" "store-query" "--queryfile" ${RMPM_QUERYFILE} "--lang" "C++" ${RMPM_PACKAGES} ${RMPM_OPTIONS}
            OUTPUT_VARIABLE __RMPM_OUT
            ERROR_VARIABLE  __RMPM_ERR)

    #error check
    if(__RMPM_ERR)
        rmpm_error_message("rmpm returned with error:\n${__RMPM_ERR}")
    endif()

    if(NOT __RMPM_OUT)
        rmpm_error_message("rmpm returned with no output (query not found?!?)\n")
    endif()

    # cleanup
    unset(__RMPM_OUT)
    unset(__RMPM_ERR)

endfunction(rmpm_query)

#
# rmpm_do env
#
function(rmpm_env ...)

    #parse args
    set(options "")
    set(oneValueArgs ENVFILE)
    set(multiValueArgs PACKAGES OPTIONS)
    cmake_parse_arguments(RMPM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGV})

    #call rmpm
    #todo :--queryfile ${RMPM_QUERYFILE}
    rmpm_status_message("rmpm env with ${ARGV}")
    execute_process(COMMAND "${RMPM_DO_CMD}" "env" "--env-format" "embed_cmake" ${RMPM_PACKAGES} ${RMPM_OPTIONS}
            OUTPUT_VARIABLE __RMPM_OUT
            ERROR_VARIABLE  __RMPM_ERR)

    #error check
    if(__RMPM_ERR)
        rmpm_error_message("rmpm returned with error:\n${__RMPM_ERR}")
    endif()

    if(NOT __RMPM_OUT)
        rmpm_error_message("rmpm returned with no output (query not found?!?)\n")
    endif()

    # load rmpm envrionment to cmake process
    rmpm_debug_message("environment from rmpm:\n${__RMPM_OUT}")
    rmpm_eval_cmd(${__RMPM_OUT})

    # write envrionment to file
    if(RMPM_ENVFILE)
        file(WRITE ${RMPM_ENVFILE} "#automatically generated rmpm environment\n")

        string(REPLACE "\n" ";" __RMPM_OUT_LIST ${__RMPM_OUT})
        foreach (line ${__RMPM_OUT_LIST})
            string(REPLACE "set(" "export " bash_line ${line})
            string(REPLACE "\")" "\"" bash_line ${bash_line})
            string(REGEX REPLACE "ENV{([^}]+)}\ " "\\1=" bash_line ${bash_line})
            string(REGEX REPLACE "ENV{([^}]+)}" "\\1" bash_line ${bash_line})
            file(APPEND ${RMPM_ENVFILE} "${bash_line}\n")
        endforeach ()
    endif()

    # cleanup
    unset(__RMPM_OUT)
    unset(__RMPM_ERR)
endfunction(rmpm_env)

#
# get key from single package
#
function(rmpm_key ...)

    #parse args
    set(options "")
    set(oneValueArgs KEY PACKAGE OUTPUT)
    set(multiValueArgs OPTIONS)
    cmake_parse_arguments(RMPM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGV})

    #call rmpm
    #todo : --queryfile ${RMPM_QUERYFILE}
    rmpm_status_message("exec: ${RMPM_DO_CMD} key ${RMPM_KEY} ${RMPM_PACKAGE} --nodep ${RMPM_OPTIONS}")
    execute_process(COMMAND ${RMPM_DO_CMD} "key" ${RMPM_KEY} ${RMPM_PACKAGE} "--nodep" ${RMPM_OPTIONS}
            OUTPUT_VARIABLE __RMPM_OUT
            ERROR_VARIABLE  __RMPM_ERR)

    #error check
    if(__RMPM_ERR)
        rmpm_error_message("rmpm returned with error:\n${__RMPM_ERR}")
    endif()

    if(NOT __RMPM_OUT)
        rmpm_error_message("rmpm returned with no output (query not found?!?)\n")
    endif()

    #generate output
    string(STRIP ${__RMPM_OUT} __RMPM_OUT)
    set(${RMPM_OUTPUT} ${__RMPM_OUT} PARENT_SCOPE)

    # cleanup
    unset(__RMPM_OUT)
    unset(__RMPM_ERR)

endfunction(rmpm_key)
