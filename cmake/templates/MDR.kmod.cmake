#
# kernel module (kmod) project:
# __configs__ : list of build configurations that are supported (e.g. sled11-x86-gcc4.x)
# __sources__ : list of source pattern (e.g. "*.cc" "test.cc")
# __headers__ : list of header pattern (e.g. "*.h" "test.hpp")
# __excludes__ : list of source exclude patterns (e.g. "test/test.cc", "texture", excludeme.cc"
# __include_dirs__ : list of include dirs for compilation
# __name_of_kernel__ : name of kernel (default is `uname -r`)
#
# Note:
# the 'find_package' commands usually set the variables
# ${<pkgname>_INCLUDE_DIRS}
# ${<pkgname>_LIBRARY_DIRS}
# ${<pkgname>_LIBRARIES}
# use them whenever you can!
#
MDR_kmod_project(__name__
#    NOINSTALL
#    CONFIGS __configs__
#    SOURCES __sources__
#    HEADERS __headers__
#    EXCLUDES __excludes__
#    INCLUDE_DIRS __include_dirs__
#    KERNEL_NAME __name_of_kernel__
)


#
# additional rules (only apply if target is supported)
#
if(${MDR___name___SUPPORTED})

### example 1: generated version header (see template directory)
### --> note: you have to set ${CMAKE_CURRENT_BINARY_DIR}/include as INCLUDE_DIR
#configure_file(MDRVersion.h.in include/MDRVersion.h @ONLY)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/MDRVersion.h DESTINATION include)

### example 2: link target to other name
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ln -sf ${PROJECT_NAME} ${PROJECT_NAME}_link)
#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_link DESTINATION bin)

endif()

