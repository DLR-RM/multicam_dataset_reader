#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# library sub-project
#
macro(MDR_library_project name)

    set(MDR_${name}_IS_LIBRARY TRUE)
    MDR_internal_setup_common_project(${name} ${ARGN})

    if(${${PROJECT_NAME}_SUPPORTED})
        #remove old caches
        #unset(${PROJECT_NAME}_SOURCES CACHE)
        #unset(${PROJECT_NAME}_INCLUDE_DIRS CACHE)
        #unset(${PROJECT_NAME}_LIBRARY_DIRS CACHE)
        #unset(${PROJECT_NAME}_LIBRARIES CACHE)

        #setup library target (shared or static [default])
        set(${PROJECT_NAME}_LIB_TYPE STATIC)
        if(${${PROJECT_NAME}_SHARED})
            set(${PROJECT_NAME}_LIB_TYPE SHARED)
        elseif(DEFINED MDR_BUILD_SHARED_LIBRARIES AND ${MDR_BUILD_SHARED_LIBRARIES} MATCHES ON)
            set(${PROJECT_NAME}_LIB_TYPE SHARED)
        endif()

        #setup global build settings (before targets)
        MDR_internal_apply_global_build_settings()

        # add library target
        add_library(${${PROJECT_NAME}_TARGET_NAME} ${${PROJECT_NAME}_LIB_TYPE} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

        # add build settings for target
        MDR_internal_apply_build_settings()

        # install rules
        if(NOT ${${PROJECT_NAME}_NOINSTALL})
            # install library
            install(TARGETS ${${PROJECT_NAME}_TARGET_NAME}
                    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
                    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
                    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
                    )

            # install header
            foreach (file ${${PROJECT_NAME}_INTERFACE})
                get_filename_component(file ${file} REALPATH)
                get_filename_component(dir ${file} DIRECTORY)
                get_filename_component(filename ${file} NAME)
                file(RELATIVE_PATH dir ${${PROJECT_NAME}_INTERFACE_STRIP_PATH} ${dir})
                install(FILES ${file}
                        DESTINATION ${INSTALL_INCLUDE_DIR}/${dir}
                        RENAME ${filename})
            endforeach()

            if(NOT ${${PROJECT_NAME}_NOEXPORT})
                # export target (add to list of library targets)
                set(MDR_LIB_TARGETS "${${PROJECT_NAME}_TARGET_NAME};${MDR_LIB_TARGETS}" PARENT_SCOPE)

                # write target properties to file
                string(TOUPPER ${${PROJECT_NAME}_TARGET_NAME} libname_upper)
                file(APPEND ${MDR_CMAKE_TARGET_PROPERTIES_FILE} "\
# properties of ${${PROJECT_NAME}_TARGET_NAME}\n\
set(MulticamDatasetReader_${libname_upper}_TYPE ${${PROJECT_NAME}_LIB_TYPE})\n\
set(MulticamDatasetReader_${libname_upper}_LANG ${${PROJECT_NAME}_CPREFIX})\n\
set(MulticamDatasetReader_${libname_upper}_DEPENDENCIES \"${${PROJECT_NAME}_INTERFACE_LIBS}\")\n\
set(MulticamDatasetReader_${libname_upper}_COMPILE_FEATURES \"\") # not implemented yet\n\
set(MulticamDatasetReader_${libname_upper}_PACKAGES \"${${PROJECT_NAME}_INTERFACE_PACKAGES}\")\n\
\n"
                )

            endif()
        endif()
    endif()

endmacro(MDR_library_project)
