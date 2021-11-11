#
# macros for rmbuild support
#
# This file is part of DLR RMC project MulticamDatasetReader
# ATTENTION: this file is part of rmbuild installation, do not edit!
#

#
# library sub-project
#
macro(MDR_java_swig_project name)

    MDR_internal_setup_common_project(${name} ${ARGN})

    if(${${PROJECT_NAME}_SUPPORTED})
        # configure swig cmake legacy api
        cmake_policy(PUSH)
        if (${CMAKE_VERSION} VERSION_GREATER "3.13")
            cmake_policy(SET CMP0078 OLD)
            cmake_policy(SET CMP0086 OLD)
        endif()

        #
        # load swig-java specific packages
        #
        find_package(SWIG)
        find_package(JNI)
        include(UseJava)
        include(UseSWIG)

        #
        # set include dirs
        # (we need them here for swig)
        #
        include_directories(${JNI_INCLUDE_DIRS})
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${MDR_INCLUDE_DIR}
                ${${PROJECT_NAME}_INCLUDE_DIRS}
                ${MDR_${${PROJECT_NAME}_CPREFIX}_IPATHS}
                ${MDR_LANG_${${PROJECT_NAME}_LANG}_IPATHS}
        )

        #setup global build settings (before targets)
        MDR_internal_apply_global_build_settings()

        #
        # setup SWIG_INTERFACE_FILES from RESOURCES
        # set source file properties
        #
        set(SWIG_INTERFACE_FILES ${${PROJECT_NAME}_RESOURCES})
        message(STATUS "SWIG_INTERFACE_FILES ${SWIG_INTERFACE_FILES}")

        foreach (swig_file ${SWIG_INTERFACE_FILES})
            get_filename_component(base_name ${swig_file} DIRECTORY)
            set_source_files_properties(${swig_file} PROPERTIES CPLUSPLUS ON)
            set_source_files_properties(${swig_file} PROPERTIES PACKAGE "${${PROJECT_NAME}_TARGET_NAME}.${base_name}")
            set_source_files_properties(${swig_file} PROPERTIES SWIG_FLAGS "-O")
        endforeach ()

        #
        # setup swig library
        #
        SWIG_ADD_LIBRARY(${${PROJECT_NAME}_TARGET_NAME}
                TYPE MODULE
                LANGUAGE JAVA
                SOURCES ${SWIG_INTERFACE_FILES}
        )

        SWIG_LINK_LIBRARIES(${${PROJECT_NAME}_TARGET_NAME} ${${PROJECT_NAME}_LIBRARIES})

        #
        # add build settings for target
        #
        MDR_internal_apply_build_settings()

        #
        # build jar
        #
        ADD_CUSTOM_COMMAND(TARGET ${${PROJECT_NAME}_TARGET_NAME} POST_BUILD
            COMMAND "${Java_JAVAC_EXECUTABLE}" *.java
            COMMAND "${Java_JAR_EXECUTABLE}" -cf ${${PROJECT_NAME}_TARGET_NAME}.jar *.class
        )

        #
        # install rules
        #
        install(TARGETS ${${PROJECT_NAME}_TARGET_NAME} LIBRARY DESTINATION java)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_TARGET_NAME}.jar DESTINATION java)

        # restore original policies
        cmake_policy(POP)
    endif()

endmacro(MDR_java_swig_project)