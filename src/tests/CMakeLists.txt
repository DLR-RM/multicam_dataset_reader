#
#  ### test project ###
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
# NOINSTALL         : don't install the library (private)
#
# Note:
# the dependencies, i.e. the cmake 'find_package' commands, usually set the variables
# ${<pkgname>_INCLUDE_DIRS}
# ${<pkgname>_LIBRARY_DIRS}
# ${<pkgname>_LIBRARIES}
# use them whenever you can!
#

MDR_executable_project(tests
        NOINSTALL # tests are usually not installed
#       AUTOMOC
#       LANG __lang__
#       CONFIGS __configs__
#       SOURCES __sources__
#       HEADERS __headers__
#       RESOURCES __resources__
#       EXCLUDES __excludes__
        INCLUDE_DIRS ${gtest_INCLUDE_DIRS} ${gdal_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src
        LIBRARY_DIRS ${gtest_LIBRARY_DIRS}
        LIBRARIES ${gtest_LIBRARIES}
        DEPENDENCIES core
)

#
# additional rules (only apply if target is supported)
#
if(${MDR_tests_SUPPORTED})

    #
    # add test command (used by 'make test')
    #  NAME: name of the test [required]
    #  COMMAND: command to execute [required]
    #  ARGS: command line arguments, e.g. -v --loglevel=debug
    #  ENVIRONMENT: additional shell environment, e.g. MYVAR=hallo
    #  WORKING_DIRECTORY: change to this directory before executing the test,
    #                     default is the current binary directory (build directory)
    MDR_add_test(NAME tests
            COMMAND __cmd__
    #        ARGS __args__
    #        ENVIRONMENT __env__
    #        WORKING_DIRECTORY __path__
    )

endif()

