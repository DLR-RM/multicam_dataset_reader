#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# kernel module macro
#
macro(MDR_internal_add_kmod)

    #
    # setup kernel, target and source
    #
    if(NOT ${PROJECT_NAME}_KERNEL_NAME)
        set(${PROJECT_NAME}_KERNEL_NAME ${CMAKE_SYSTEM_VERSION})
    endif()

    set(${PROJECT_NAME}_KERNEL_DIR "/lib/modules/${${PROJECT_NAME}_KERNEL_NAME}/build")
    set(KMOD_NAME ${${PROJECT_NAME}_TARGET_NAME})

    foreach(_src_abs ${${PROJECT_NAME}_SOURCES})
        get_filename_component(_src ${_src_abs} NAME)
        list(APPEND KMOD_SOURCES ${_src})
    endforeach()

    #
    # gen output files
    #
    set(KMOD_KOFILE "${KMOD_NAME}.ko")
    set(KMOD_OFILE "${KMOD_NAME}.o")
    set(KMOD_MOD_CFILE "${KMOD_NAME}.mod.c")
    set(KMOD_MOD_OFILE "${KMOD_NAME}.mod.o")
    string(REGEX REPLACE "\\.c(;|$)" ".o\\1" KMOD_OBJECTS "${KMOD_SOURCES}")

    set(KMOD_OUTPUTS ${KMOD_KOFILE} ${KMOD_OFILE} ${KMOD_OBJECTS} ${KMOD_MOD_CFILE} ${KMOD_MOD_OFILE})
    string(REGEX REPLACE "([^;]+)(;|$)" ".\\1.cmd\\2" KMOD_CMDFILES "${KMOD_OUTPUTS}")

    message(STATUS "outputs: ${KMOD_OUTPUTS}")


    # generate include paths
    #
    foreach(_include_dir ${${PROJECT_NAME}_INCLUDE_DIRS})
        if(IS_ABSOLUTE${_include_dir})
            list(APPEND KMOD_INCLUDES -I${_include_dir})
        else()
            list(APPEND KMOD_INCLUDES -I$(src)/${_include_dir})
        endif()
    endforeach()

    #
    # generate Kbuild file
    #

    string(REPLACE ";" " " KMOD_OBJECTS "${KMOD_OBJECTS}")
    configure_file(
            ${PROJECT_SOURCE_DIR}/Kbuild.in
            ${PROJECT_BINARY_DIR}/Kbuild
            UNIX
    )

    #
    # add command and target
    #
    add_custom_command(
        OUTPUT ${KMOD_OUTPUTS} ${KMOD_CMDFILES} .tmp_versions Module.symvers modules.order
        COMMAND ${CMAKE_MAKE_PROGRAM} -C ${${PROJECT_NAME}_KERNEL_DIR} M=${PROJECT_BINARY_DIR} SRCDIR=${PROJECT_SOURCE_DIR} modules
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        USES_TERMINAL
        COMMENT "building kernel module ${${PROJECT_NAME}_TARGET_NAME}"
    )

    add_custom_target(
        ${${PROJECT_NAME}_TARGET_NAME}
        ALL
        DEPENDS ${KMOD_KOFILE}
        SOURCES ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS}
    )

    #
    # add install target
    #
    if(NOT ${${PROJECT_NAME}_NOINSTALL})
        install(
            FILES ${PROJECT_BINARY_DIR}/${KMOD_KOFILE}
            DESTINATION kernel/${CMAKE_SYSTEM_VERSION}
        )
    endif()

    #add target to targets.yaml
    file(APPEND ${MDR_TARGET_FILE} "${${PROJECT_NAME}_TARGET_NAME}: ${MDR_ROOT_RELATIVE}\n")

endmacro(MDR_internal_add_kmod)

#
# kmod sub-project
#
macro(MDR_kmod_project name)
    MDR_internal_setup_common_project(${name} ${ARGN})

    if(${${PROJECT_NAME}_SUPPORTED})
        MDR_internal_add_kmod()
    endif()

endmacro(MDR_kmod_project)