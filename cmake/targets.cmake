#
# macros for creating targets
#

macro(set_install_of_target name)
    # install target
    install(TARGETS ${name}
            RUNTIME DESTINATION ${MDR_BUILD_DIR}/bin
            ARCHIVE DESTINATION ${MDR_BUILD_DIR}/lib
            LIBRARY DESTINATION ${MDR_BUILD_DIR}/lib
            )
endmacro(set_install_of_target)

#
# library target
#
function(add_library_target name)
    cmake_parse_arguments(
            add_library
            "STATIC;SHARED"
            ""
            "SOURCE_PATTERN;HEADER_PATTERN;INCLUDE_DIRS;LIBRARY_DIRS;LIBRARIES;DEPENDENCIES"
            ${ARGN}
    )

    message(STATUS "Add library " ${name})
    if (DEBUG)
        dump_cmake_variables("add_library")
    endif ()

    # check STATIC/SHARED flags
    if(${add_library_SHARED})
        message(STATUS "${name} set to be SHARED")
        set(${name}_BUILD_FLAGS ${${name}_BUILD_FLAGS} SHARED)
    endif()

    if(${add_library_STATIC})
        message(STATUS "${name} set to be STATIC")
        set(${name}_BUILD_FLAGS ${${name}_BUILD_FLAGS} STATIC)
    endif()

    # get files
    foreach(pattern ${add_library_SOURCE_PATTERN})
        find_files_by_pattern(${pattern} ${name}_SOURCES_new)
        set(${name}_SOURCES ${${name}_SOURCES} ${${name}_SOURCES_new})
    endforeach()
    foreach(pattern ${add_library_HEADER_PATTERN})
        find_files_by_pattern(${pattern} ${name}_HEADERS_new)
        set(${name}_HEADERS ${${name}_HEADERS} ${${name}_HEADERS_new})
    endforeach()

    if(DEBUG)
        message(STATUS ${${name}_SOURCES})
        message(STATUS ${${name}_HEADERS})
    endif()

    # create lib
    add_library(${name} ${${name}_BUILD_FLAGS} ${${name}_SOURCES} ${${name}_HEADERS})

    # add directories
    include_directories(${name} PUBLIC ${add_library_INCLUDE_DIRS})

    # dependencies
    if(add_library_LIBRARIES)
        foreach(item ${add_library_LIBRARIES})
            message(STATUS "Adding external dependencies: ${item}")
            target_link_libraries(${name} PUBLIC ${item})
        endforeach()
    endif()
    if(add_library_DEPENDENCIES)
        foreach(item ${add_library_DEPENDENCIES})
            message(STATUS "Adding dependencies: ${item}")
            add_dependencies(${name} ${item})
            target_link_libraries(${name} PUBLIC ${item})
        endforeach()
    endif()

    if(add_library_LIBRARY_DIRS)
        link_directories(${add_library_LIBRARY_DIRS})
    endif()

    # install
    set_install_of_target(${name})
endfunction(add_library_target)

#
# executable target
#
function(add_executable_target name)
    cmake_parse_arguments(
            add_executable
            ""
            ""
            "SOURCE_PATTERN;HEADER_PATTERN;INCLUDE_DIRS;LIBRARY_DIRS;LIBRARIES;DEPENDENCIES"
            ${ARGN}
    )

    message(STATUS "Add executable " ${name})
    if (DEBUG)
        dump_cmake_variables("add_executable")
    endif ()

    # get files
    foreach(pattern ${add_executable_SOURCE_PATTERN})
        find_files_by_pattern(${pattern} ${name}_SOURCES_new)
        set(${name}_SOURCES ${${name}_SOURCES} ${${name}_SOURCES_new})
    endforeach()
    foreach(pattern ${add_executable_HEADER_PATTERN})
        find_files_by_pattern(${pattern} ${name}_HEADERS_new)
        set(${name}_HEADERS ${${name}_HEADERS} ${${name}_HEADERS_new})
    endforeach()

    if(DEBUG)
        message(STATUS ${${name}_SOURCES})
        message(STATUS ${${name}_HEADERS})
    endif()

    # create lib
    add_executable(${name} ${${name}_SOURCES} ${${name}_HEADERS})

    # add directories
    include_directories(${name} PUBLIC ${add_executable_INCLUDE_DIRS})

    # dependencies
    if(add_executable_LIBRARIES)
        foreach(item ${add_executable_LIBRARIES})
            message(STATUS "Adding external dependencies: ${item}")
            target_link_libraries(${name} PUBLIC ${item})
        endforeach()
    endif()
    if(add_executable_DEPENDENCIES)
        foreach(item ${add_executable_DEPENDENCIES})
            message(STATUS "Adding internal dependencies: ${item}")
            add_dependencies(${name} ${item})
            target_link_libraries(${name} PUBLIC ${item})
        endforeach()
    endif()

    if(add_executable_LIBRARY_DIRS)
        link_directories(${add_executable_LIBRARY_DIRS})
    endif()

    # install
    set_install_of_target(${name})
endfunction(add_executable_target)