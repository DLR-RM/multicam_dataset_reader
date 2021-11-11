#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# create a build target as new Xx cmake project
# + calls project with PROJECT_NAME = MDR_${name}
# + sets ${PROJECT_NAME}_ROOT_DIR ${PROJECT_NAME}_ROOT_RELATIVE
# + sets ${PROJECT_NAME}_TARGET_NAME
#
macro(MDR_internal_setup_project name)

    #cmake_minimum_required(VERSION 3.0)
    project(MDR_${name})
    message(STATUS "creating MDR subproject ${name} as ${PROJECT_NAME}")

    #add root dir of project as variable
    get_filename_component(${PROJECT_NAME}_ROOT_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
    set(${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_ROOT_DIR}" CACHE INTERNAL "${PROJECT_NAME} root directory")

    #include common setup, if not already done
    if(NOT ${MDR_SETUP_DONE})
        include(../cmake/MDRSetupCommon.cmake)
    endif()

    #setup target name
    set(${PROJECT_NAME}_TARGET_NAME "${name}" CACHE INTERNAL "${PROJECT_NAME} target name")

    #compute relative path from project root to sub-project root
    file(RELATIVE_PATH ${PROJECT_NAME}_ROOT_RELATIVE ${MDR_ROOT_DIR} ${PROJECT_SOURCE_DIR})

endmacro(MDR_internal_setup_project)

#
# parse project arguments
#
macro(MDR_internal_parse_project_args prefix)

    #parse args
    set(options AUTOMOC SHARED STATIC NOINSTALL NOEXPORT)
    set(oneValueArgs LANG KERNEL_NAME INTERFACE_STRIP_PATH)
    set(multiValueArgs  CONFIGS SOURCES RESOURCES HEADERS EXCLUDES
                        INCLUDE_DIRS LIBRARY_DIRS LIBRARIES DEPENDENCIES
                        INTERFACE INTERFACE_PACKAGES COMMAND)
    cmake_parse_arguments(${prefix} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    #debug
    debug_message("LANG ${${PROJECT_NAME}_LANG}")
    debug_message("KERNEL_NAME ${${PROJECT_NAME}_KERNEL_NAME}")
    debug_message("CONFIGS ${${PROJECT_NAME}_CONFIGS}")
    debug_message("SOURCES ${${PROJECT_NAME}_SOURCES}")
    debug_message("RESOURCES ${${PROJECT_NAME}_RESOURCES}")
    debug_message("HEADERS ${${PROJECT_NAME}_HEADERS}")
    debug_message("EXCLUDES ${${PROJECT_NAME}_EXCLUDES}")
    debug_message("INTERFACE ${${PROJECT_NAME}_INTERFACE}")
    debug_message("INTERFACE_STRIP_PATH ${${PROJECT_NAME}_INTERFACE_STRIP_PATH}")
    debug_message("INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS}")
    debug_message("LIBRARY_DIRS ${${PROJECT_NAME}_LIBRARY_DIRS}")
    debug_message("LIBRARIES ${${PROJECT_NAME}_LIBRARIES}")
    debug_message("DEPENDENCIES ${${PROJECT_NAME}_DEPENDENCIES}")
    debug_message("INTERFACE_PACKAGES ${${PROJECT_NAME}_INTERFACE_PACKAGES}")

    # set strip path to default
    if(NOT ${PROJECT_NAME}_INTERFACE_STRIP_PATH)
        get_filename_component(${PROJECT_NAME}_INTERFACE_STRIP_PATH "${${PROJECT_NAME}_ROOT_DIR}/.." ABSOLUTE)
    else()
        get_filename_component(${PROJECT_NAME}_INTERFACE_STRIP_PATH "${${PROJECT_NAME}_INTERFACE_STRIP_PATH}" ABSOLUTE)
    endif()

endmacro(MDR_internal_parse_project_args)

#
# is build config supported
#
macro(MDR_internal_check_config_supported)
    #check config support
    set(${PROJECT_NAME}_SUPPORTED FALSE)
    if(DEFINED ${PROJECT_NAME}_CONFIGS)
        foreach(_config ${${PROJECT_NAME}_CONFIGS})
            string(REGEX MATCH ${_config} _match ${RMBUILD_CONFIG})
            #message("dbg :: match ${_config}  ?= ${RMBUILD_CONFIG} := ${_match}")
            if(NOT "${_match}" STREQUAL "")
                set(${PROJECT_NAME}_SUPPORTED TRUE)
                break()
            endif()
        endforeach()
        message(STATUS " ++ project supported=${${PROJECT_NAME}_SUPPORTED}")

    else()
        message(STATUS " ++ project supported=TRUE (no supported configurations specified)")
        set(${PROJECT_NAME}_SUPPORTED TRUE)
    endif()

    #set(${PROJECT_NAME}_SUPPORTED ${${PROJECT_NAME}_SUPPORTED} CACHE BOOL "${PROJECT_NAME} supported")

    if(${${PROJECT_NAME}_SUPPORTED})
        #add target to targets.yaml
        file(APPEND ${MDR_TARGET_FILE} "${${PROJECT_NAME}_TARGET_NAME}: ${${PROJECT_NAME}_ROOT_RELATIVE}\n")
    endif(${${PROJECT_NAME}_SUPPORTED})

endmacro()

#
# setup language flags: ipaths, lpaths, libs, defines, flags, lflags
#
macro(MDR_internal_set_lang)

    #fallback to default language
    if(NOT ${PROJECT_NAME}_LANG)
        set(${PROJECT_NAME}_LANG ${MDR_DEFAULT_LANG})
    endif()

    #lang is always in capital letters
    string(TOUPPER "${${PROJECT_NAME}_LANG}" ${PROJECT_NAME}_LANG)

    #is language supported?
    if(NOT ${MDR_LANG_${${PROJECT_NAME}_LANG}_SUPPORTED})
        message(FATAL_ERROR "${PROJECT_NAME}:: language '${${PROJECT_NAME}_LANG}' not supported by platform!")
    endif()

    #
    # setup C / C++ specific tool prefix
    #
    if("${${PROJECT_NAME}_LANG}" STREQUAL "C")
        set(${PROJECT_NAME}_CPREFIX "C")

    else()
        set(${PROJECT_NAME}_CPREFIX "CXX")

    endif()

    message(STATUS " ++ lang=${${PROJECT_NAME}_LANG}")

endmacro(MDR_internal_set_lang)

#
# filter sources, headers, and interfaces from patterns
#
macro(MDR_internal_filter_sources)
    if(NOT ${PROJECT_NAME}_SOURCES)
        set(${PROJECT_NAME}_SOURCES ${MDR_DEFAULT_SOURCE_PATTERN})
    endif()

    debug_message("source files (pattern): ${${PROJECT_NAME}_SOURCES}")
    file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES})
    debug_message("source files (raw): ${${PROJECT_NAME}_SOURCES}")

    if(NOT DEFINED ${PROJECT_NAME}_HEADERS)
        set(${PROJECT_NAME}_HEADERS ${MDR_DEFAULT_HEADER_PATTERN})
    endif()

    debug_message("header files (pattern): ${${PROJECT_NAME}_HEADERS}")
    file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS})
    debug_message("header files (raw): ${${PROJECT_NAME}_HEADERS}")

    #find excludes
    if(DEFINED ${PROJECT_NAME}_EXCLUDES)
        #list of source files to exclude
        foreach(_src ${${PROJECT_NAME}_SOURCES})
            foreach(_exclude ${${PROJECT_NAME}_EXCLUDES})
                string(REGEX MATCH ${_exclude} _match ${_src})
                if(NOT "${_match}" STREQUAL "")
                    #message("${_exclude} match ${_src} = ${_match}")
                    list(APPEND _filtered_srcs ${_src})
                endif()
                unset(_match)
            endforeach(_exclude)
        endforeach(_src)

        #list of header files to exclude
        foreach(_header ${${PROJECT_NAME}_HEADERS})
            foreach(_exclude ${${PROJECT_NAME}_EXCLUDES})
                string(REGEX MATCH ${_exclude} _match ${_header})
                if(NOT "${_match}" STREQUAL "")
                    #message("${_exclude} match ${_src} = ${_match}")
                    list(APPEND _filtered_headers ${_header})
                endif()
                unset(_match)
            endforeach(_exclude)
        endforeach(_header)

    endif()

    #remove excludes from sources
    foreach(_f ${_filtered_srcs})
        list(REMOVE_ITEM ${PROJECT_NAME}_SOURCES ${_f})
    endforeach(_f)
    unset(_filtered_srcs)

    #remove excludes from headers
    foreach(_f ${_filtered_headers})
        list(REMOVE_ITEM ${PROJECT_NAME}_HEADERS ${_f})
    endforeach(_f)
    unset(_filtered_headers)

    debug_message("source files (filtered): ${${PROJECT_NAME}_SOURCES}")
    debug_message("header files (filtered): ${${PROJECT_NAME}_HEADERS}")

    #
    # setup interface headers
    #
    if(NOT DEFINED ${PROJECT_NAME}_INTERFACE)
        set(${PROJECT_NAME}_INTERFACE ${${PROJECT_NAME}_HEADERS})

    else()
        debug_message("interface files (pattern): ${${PROJECT_NAME}_INTERFACE}")
        file(GLOB_RECURSE ${PROJECT_NAME}_INTERFACE ${${PROJECT_NAME}_INTERFACE})
        debug_message("interface files (raw): ${${PROJECT_NAME}_INTERFACE}")

    endif()

endmacro(MDR_internal_filter_sources)

#
# applies global build settings (use inside of project macros)
# note: before target definition
#
macro(MDR_internal_apply_global_build_settings)
    #
    # setup library dirs
    # (must be called before target definition)
    #
    debug_message("adding library directories ${${PROJECT_NAME}_LIBRARY_DIRS}")
    link_directories(
        ${MDR_LIBRARY_DIR}
        ${${PROJECT_NAME}_LIBRARY_DIRS}
        ${MDR_${${PROJECT_NAME}_CPREFIX}_LPATHS}
        ${MDR_LANG_${${PROJECT_NAME}_LANG}_LPATHS}
    )
endmacro(MDR_internal_apply_global_build_settings)

#
# applies build settings (use inside of project macros)
# note: assumes that a target is set
#
macro(MDR_internal_apply_build_settings)
    #
    # setup compile flags (incl. lang)
    #
    set(${PROJECT_NAME}_FLAGS
        ${MDR_COMPILER_FLAGS}
        ${MDR_${${PROJECT_NAME}_CPREFIX}_FLAGS}
        ${MDR_LANG_${${PROJECT_NAME}_LANG}_FLAGS}
    )

    target_compile_options(${${PROJECT_NAME}_TARGET_NAME} PUBLIC
        "${${PROJECT_NAME}_FLAGS}"
    )

    #
    # setup defines
    #
    target_compile_definitions(${${PROJECT_NAME}_TARGET_NAME} PUBLIC
        ${MDR_DEFINES}
        ${MDR_${${PROJECT_NAME}_CPREFIX}_DEFINES}
        ${MDR_LANG_${${PROJECT_NAME}_LANG}_DEFINES}
    )

    #
    # setup linker flags (include flags)
    #
    set(${PROJECT_NAME}_LFLAGS
        ${${PROJECT_NAME}_FLAGS}
        ${MDR_COMPILER_LFLAGS}
        ${MDR_${${PROJECT_NAME}_CPREFIX}_LFLAGS}
        ${MDR_LANG_${${PROJECT_NAME}_LANG}_LFLAGS}
    )

    if(${PROJECT_NAME}_LFLAGS)
        # set_target_properties doesn't accept lists [-(
        string(REPLACE ";" " " ${PROJECT_NAME}_LFLAGS_STR "${${PROJECT_NAME}_LFLAGS}")

        # target_link_options is not supported by cmake 3.10, so we have to use set_target_properties
        # however, set_target_properties doesn't accept empty properties
        set_target_properties(${${PROJECT_NAME}_TARGET_NAME} PROPERTIES LINK_FLAGS
            ${${PROJECT_NAME}_LFLAGS_STR}
        )
    endif()

    #
    # setup included dirs
    #
    debug_message("adding include directories ${${PROJECT_NAME}_INCLUDE_DIRS}")
    target_include_directories(${${PROJECT_NAME}_TARGET_NAME} PUBLIC
        ${MDR_INCLUDE_DIR}
        ${${PROJECT_NAME}_INCLUDE_DIRS}
        ${MDR_${${PROJECT_NAME}_CPREFIX}_IPATHS}
        ${MDR_LANG_${${PROJECT_NAME}_LANG}_IPATHS}
    )

    #
    # add link libraries
    #
    if(DEFINED ${PROJECT_NAME}_DEPENDENCIES)
        foreach(__dep ${${PROJECT_NAME}_DEPENDENCIES})
            if(${MDR_${__dep}_IS_LIBRARY})
                list(APPEND ${PROJECT_NAME}_INTERFACE_LIBS ${MDR_${__dep}_TARGET_NAME})
                target_link_libraries(${${PROJECT_NAME}_TARGET_NAME}
                        ${MDR_${__dep}_TARGET_NAME}
                    )
            else()
                add_dependencies(${${PROJECT_NAME}_TARGET_NAME}
                        ${MDR_${__dep}_TARGET_NAME}
                    )
            endif()
        endforeach()
    endif()

    target_link_libraries(${${PROJECT_NAME}_TARGET_NAME}
        ${${PROJECT_NAME}_LIBRARIES}
        ${MDR_${${PROJECT_NAME}_CPREFIX}_LIBS}
        ${MDR_LANG_${${PROJECT_NAME}_LANG}_LIBS}
    )

    foreach(__lib ${${PROJECT_NAME}_LIBRARIES})
        if(IS_ABSOLUTE "${__lib}")
            get_filename_component(__lib "${__lib}" NAME)
        endif()
        list(APPEND ${PROJECT_NAME}_INTERFACE_LIBS ${__lib})
    endforeach()


    # Required on Unix OS family to be able to be linked into shared libraries.
    set_target_properties(${${PROJECT_NAME}_TARGET_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

    #install rules
#    if(NOT ${${PROJECT_NAME}_NOINSTALL})
#        install(TARGETS ${${PROJECT_NAME}_TARGET_NAME}
#                RUNTIME DESTINATION ${INSTALL_BIN_DIR}
#                ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
#                LIBRARY DESTINATION ${INSTALL_LIB_DIR}
#        )
#    endif()

endmacro(MDR_internal_apply_build_settings)

#
# create common (lib or exe) project
#
macro(MDR_internal_setup_common_project name)
    if(MDR_DEPENDS_MODE)
        #just for savety reasons...
        set(${PROJECT_NAME}_SUPPORTED FALSE)

        #generate target depends
        MDR_internal_get_project_depends(${name} ${ARGN})

    elseif(NOT DEFINED MDR_${name}_FOUND)
        #setup basic project
        MDR_internal_setup_project(${name})

        #remove old caches
        unset(${PROJECT_NAME}_SOURCES CACHE)
        unset(${PROJECT_NAME}_INCLUDE_DIRS CACHE)
        unset(${PROJECT_NAME}_LIBRARY_DIRS CACHE)
        unset(${PROJECT_NAME}_LIBRARIES CACHE)

        #parse args
        MDR_internal_parse_project_args(${PROJECT_NAME} ${ARGN})

        if(NOT ${PROJECT_NAME}_IS_LIBRARY)
            set(${PROJECT_NAME}_IS_LIBRARY FALSE)
        endif()
        set(${PROJECT_NAME}_IS_LIBRARY ${${PROJECT_NAME}_IS_LIBRARY} PARENT_SCOPE)

        #check config support
        MDR_internal_check_config_supported()

        #do common default operations:
        # 1. collect sources
        # 2. collect dependecies
        if(${${PROJECT_NAME}_SUPPORTED})
            #setup target
            MDR_internal_set_lang()
            MDR_internal_filter_sources()
            MDR_internal_add_dependencies()

            #handle options
            if(${${PROJECT_NAME}_AUTOMOC})
                message(STATUS " ++ activating qt automoc")
                set(CMAKE_AUTOMOC TRUE)
            else()
                set(CMAKE_AUTOMOC FALSE)
            endif()

        endif()

        set(${PROJECT_NAME}_FOUND TRUE)
        set(${PROJECT_NAME}_FOUND TRUE PARENT_SCOPE)

        #export variables for dependecies
        set(${PROJECT_NAME}_SOURCES "${${PROJECT_NAME}_SOURCES}" CACHE INTERNAL "${PROJECT_NAME} sources")
        set(${PROJECT_NAME}_INCLUDE_DIRS "${${PROJECT_NAME}_INCLUDE_DIRS}" CACHE INTERNAL "${PROJECT_NAME} exported include dirs")
        set(${PROJECT_NAME}_LIBRARY_DIRS "${${PROJECT_NAME}_LIBRARY_DIRS}" CACHE INTERNAL "${PROJECT_NAME} exported library dirs")
        set(${PROJECT_NAME}_LIBRARIES "${${PROJECT_NAME}_LIBRARIES}" CACHE INTERNAL "${PROJECT_NAME} exported libraries")

    endif()

endmacro(MDR_internal_setup_common_project)
