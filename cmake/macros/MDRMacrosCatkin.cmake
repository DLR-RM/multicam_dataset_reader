#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#


#
# catkin sub-project
#
macro(MDR_catkin_project name)

    MDR_internal_setup_common_project(${name} ${ARGN})

    if(${${PROJECT_NAME}_SUPPORTED})

    endif()

endmacro(MDR_library_project)