#
# MDRSetupConfiguration.cmake - common cmake setup and platform selection for project MulticamDatasetReader
#

#
# prepare configuration:
# add cmake configuration parts before packages are loaded
# (e.g. package switches and config options)
#

#example: boost should use static libs
# set(Boost_USE_STATIC_LIBS ON)


#
# load rmbuild configuration setup
# ATTENTION: do not change this
#
if(EXISTS ${MDR_CONFIG_SETUP_DIR}/MDRConfigurationInfo.cmake)
  include(${MDR_CONFIG_SETUP_DIR}/MDRConfigurationInfo.cmake)

else()
  message(FATAL_ERROR "rmbuild configuration info for cmake not found!")

endif()
#
# end (rmbuild configuration setup)
#

#
# post configuration:
# add cmake configuration parts that require rmbuild package info
# (e.g. addtional package variables
#

#
# eof
#