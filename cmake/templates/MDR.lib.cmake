#
#  ### library project ###
# configuration variables:
# __lang__          : language of the project (c, c++, c++gnu, c++11, c++11gnu, c++14, c++14gnu, c++17, c+17gnu)
#                       [str, default is project default]
# __configs__       : supported build configurations, can also be a partial name (e.g. sled11-x86-gcc4.x or osl)
#                       [list, default is 'all']
# __sources__       : list of sources, can contain wildcard pattern (e.g. "*.cc" "test.cc")
#                       [list, default is project default pattern]
# __headers__       : list of header files, can contain wildcard pattern (e.g. "*.h" "test.hpp")
#                       [list, default is project default pattern]
# __resources__     : list of other resources, can contain wildcard pattern (e.g. "jsnstream.i" in swig projects)
#                       [list, default is none]
# __excludes__      : list of exluded source and header files, can contain patterns (e.g. "test/test.cc", "texture", excludeme.cc"
#                       [list, default is none]
# __interface__     : list of public header files, i.e. headers that are installed as API, can contain wildcard pattern (e.g. "*.h" "test.hpp")
#                       [list, default ist same as HEADERS]
# __strip_path__    : strip path for installing header files (e.g. "mypath", ${Boost_INCLUDE_DIR})
#                       [path, default is "${${PROJECT_NAME}_ROOT_DIR}/.."]
# __include_dirs__  : list of required include dirs without dependencies (e.g. "mypath", ${Boost_INCLUDE_DIR})
#                       [list, default is project none]
# __library_dirs__  :  list of required library dirs without dependencies (e.g. "mypath", ${Boost_LIBRARY_DIR})
#                       [list, default is project none]
# __libraries__     : list of required libraries without dependencies (e.g. "mylib", ${Boost_LIBRARIES})
#                       [list, default ist project default]
# __depends__       : list of required MulticamDatasetReader targets, automatially adds target to  include dirs, library dirs, and libraries.
#                     this includes the sub-project and all its external dependencies!!!
#                       [list, default ist project default]
#
# configuration flags:
# AUTOMOC           : execute qt4/5 automoc and include generated files (requires fount qt)
# SHARED/STATIC     : force creation of shared or static library
# NOINSTALL         : don't install the library (private)
# NOEXPORT          : install library, but don't expose it to dependencies (protected)
#
# Note:
# the dependencies, i.e. the cmake 'find_package' commands, usually set the variables
# ${<pkgname>_INCLUDE_DIRS}
# ${<pkgname>_LIBRARY_DIRS}
# ${<pkgname>_LIBRARIES}
# use them whenever you can!
#

MDR_library_project(__name__
#       AUTOMOC SHARED STATIC NOINSTALL NOEXPORT
#       LANG __lang__
#       CONFIGS __configs__
#       SOURCES __sources__
#       HEADERS __headers__
#       RESOURCES __resources__
#       EXCLUDES __excludes__
#       INTERFACE __interface_headers__
#       INTERFACE_STRIP_PATH __strip_path__
#       INCLUDE_DIRS __include_dirs__
#       LIBRARY_DIRS __library_dirs__
#       LIBRARIES __libraries__
#       DEPENDENCIES __depends__
)

#
# additional rules (only apply if target is supported)
#
if(${MDR___name___SUPPORTED})

    ### example 1: link target to other name
    #add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ln -sf lib${PROJECT_NAME}.a lib${PROJECT_NAME}_link.a)
    #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}_link.a DESTINATION lib)

    ### example 2: generated version header (see template directory)
    ### --> note: you have to set ${CMAKE_CURRENT_BINARY_DIR}/include as INCLUDE_DIR
    #configure_file(${MDR_CMAKE_DIR}/templates/MDRVersion.h.in include/MDRVersion.h @ONLY)
    #install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/MDRVersion.h DESTINATION include)

endif()

