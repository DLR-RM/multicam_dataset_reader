#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# generate variables for project dependencies
#
macro(MDR_internal_get_project_depends name)

    #parse
    MDR_internal_parse_project_args(MDR_${name}_PRE ${ARGN})

    #add target to global list
    get_property(MDR_BUILD_TARGETS GLOBAL PROPERTY MDR_BUILD_TARGETS)
    list(APPEND MDR_BUILD_TARGETS ${name})
    set_property(GLOBAL PROPERTY MDR_BUILD_TARGETS ${MDR_BUILD_TARGETS})

    set(MDR_${name}_PRE_TARGET_DIR ${CMAKE_CURRENT_LIST_DIR})
    set(MDR_${name}_PRE_DEPENDENCIES ${MDR_${name}_PRE_DEPENDENCIES})

endmacro(MDR_internal_get_project_depends)

#
# generate sorted list of targets w.r.t. dependencies
#
macro(MDR_internal_sort_targets)

    message(STATUS "sorting targets by dependencies ...")

    #generate per-target-dependencies variables
    set(MDR_BUILD_TARGET_DIRS_SORTED)
    set_property(GLOBAL PROPERTY MDR_BUILD_TARGETS)

    set(MDR_DEPENDS_MODE TRUE)
    foreach(__mdr_target_dir ${ARGN})
        debug_message(STATUS ">> adding target ${__mdr_target_dir}")
        include(${__mdr_target_dir}/CMakeLists.txt)
    endforeach()
    set(MDR_DEPENDS_MODE FALSE)

    #loop and sort targets
    get_property(MDR_BUILD_TARGETS GLOBAL PROPERTY MDR_BUILD_TARGETS)
    set(__mdr_have_changes TRUE)
    while(__mdr_have_changes)
        set(__mdr_have_changes FALSE)
        set(__mdr_remaining_targets)
        foreach(__mdr_target ${MDR_BUILD_TARGETS})
            set(__mdr_target_dir ${MDR_${__mdr_target}_PRE_TARGET_DIR})
            debug_message(STATUS "processing ${__mdr_target} in ${__mdr_target_dir}")
            list(FIND MDR_BUILD_TARGET_DIRS_SORTED ${__mdr_target_dir} __mdr_index)
            if(${__mdr_index} EQUAL -1)
                debug_message(STATUS "${__mdr_target_dir} not in sorted list -> try to insert (d: ${MDR_${__mdr_target}_PRE_DEPENDENCIES})")

                # loop depends and check if they are already inserted
                set(__mdr_deps_okay TRUE)
                foreach(__mdr_dep ${MDR_${__mdr_target}_PRE_DEPENDENCIES})
                    set(__mdr_dep_target_dir ${MDR_${__mdr_dep}_PRE_TARGET_DIR})
                    list(FIND MDR_BUILD_TARGET_DIRS_SORTED ${__mdr_dep_target_dir} __mdr_index)
                    if(${__mdr_index} EQUAL -1)
                        set(__mdr_deps_okay FALSE)
                        break()
                    endif()
                endforeach()

                #insert if deps okay
                if(${__mdr_deps_okay})
                    set(__mdr_have_changes TRUE)
                    list(APPEND MDR_BUILD_TARGET_DIRS_SORTED ${__mdr_target_dir})

                else()
                    list( APPEND __mdr_remaining_targets ${__mdr_target_dir})

                endif()

                #debug_message(STATUS "current list: ${MDR_BUILD_TARGET_DIRS_SORTED}")

            endif()
        endforeach()
    endwhile()

    debug_message(STATUS "final list: ${MDR_BUILD_TARGET_DIRS_SORTED}")

    #error checking: do we have any dependency circles
    list(LENGTH __mdr_remaining_targets __mdr_length_remaining_targets)
    if(NOT ${__mdr_length_remaining_targets} EQUAL 0)
        message(STATUS " sort targets failed... dependency circles detected!")
        message(STATUS "can't resolve dependecies for the following target directories:")
        foreach(__mdr_unresolved ${__mdr_remaining_targets})
            message(STATUS " ** ${__mdr_unresolved}")
        endforeach()
        message(FATAL_ERROR "... abort")
    endif()

    message(STATUS " ... done")

endmacro(MDR_internal_sort_targets)

#
# add dependencies
#
macro(MDR_internal_add_dependencies)
    message(STATUS " ++ adding DEPENDENCIES ${${PROJECT_NAME}_DEPENDENCIES}")
    if(DEFINED ${PROJECT_NAME}_DEPENDENCIES)
        foreach(__dep ${${PROJECT_NAME}_DEPENDENCIES})
            #message("adding dependency to project ${__dep}")
            if(NOT DEFINED MDR_${__dep}_FOUND)
                message(FATAL_ERROR "required dependecy ${__dep} is not loaded ... maybe you have to change the ordering of the targets")
            endif()

            if(DEFINED MDR_${__dep}_SUPPORTED AND NOT ${MDR_${__dep}_SUPPORTED})
                message(FATAL_ERROR "required dependecy ${__dep} is not supported for this configuration")
            endif()

            if(DEFINED MDR_${__dep}_INCLUDE_DIRS)
                #message("adding ipaths ${MDR_${__dep}_INCLUDE_DIRS}")
                list(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${MDR_${__dep}_INCLUDE_DIRS})
            endif()

            if(DEFINED MDR_${__dep}_LIBRARY_DIRS)
                #message("adding lpaths ${MDR_${__dep}_LIBRARY_DIRS}")
                list(APPEND ${PROJECT_NAME}_LIBRARY_DIRS ${MDR_${__dep}_LIBRARY_DIRS})
            endif()

            if(DEFINED MDR_${__dep}_LIBRARIES)
                #message("adding libs ${MDR_${__dep}_LIBRARIES}")
                list(APPEND ${PROJECT_NAME}_LIBRARIES ${MDR_${__dep}_LIBRARIES})
            endif()

        endforeach(__dep)
    endif()
endmacro(MDR_internal_add_dependencies)
