#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# for macro arugment parsing
#
include(CMakeParseArguments)

#
# debug messages
#
#set(MDR_DEBUG ON)
function(debug_message)
    if(MDR_DEBUG MATCHES ON)
        message(STATUS "DEBUG: ${ARGN}")
    endif()
endfunction(debug_message)

function(info_message)
    message(STATUS "INFO: ${ARGN}")
endfunction(info_message)
