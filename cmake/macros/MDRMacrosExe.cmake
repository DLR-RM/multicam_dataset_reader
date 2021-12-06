#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# executable sub-project
#
macro(MDR_executable_project name)
    MDR_internal_setup_common_project(${name} ${ARGN})

    if(${${PROJECT_NAME}_SUPPORTED})

        #setup global build settings (before targets)
        MDR_internal_apply_global_build_settings()

        # add executable target
        add_executable(${${PROJECT_NAME}_TARGET_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

        # add build settings for target
        MDR_internal_apply_build_settings()

        # install rules
        if(NOT ${${PROJECT_NAME}_NOINSTALL})
            install(TARGETS ${${PROJECT_NAME}_TARGET_NAME}
                    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
                    )
        endif()

    endif()

endmacro(MDR_executable_project)
