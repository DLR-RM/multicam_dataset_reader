# -*- coding: utf-8 -*-

import os
from conans import ConanFile, tools, CMake
from cissy.conantools import apply_patches

class Multicam_DatasetConan(ConanFile):

    # ######################################################### #
    #                package meta-information
    # ######################################################### #

    # * package name (string) [required]
    #       name of the package
    name = "Multicam_Dataset"

    # * author (string) [required]
    #       package provider (and maintainer)
    author = "sewt_ma <marco.sewtz@dlr.de>"

    # * developers (list) [optional]
    #       additional list of package developers, excluding the 'author'
    #       e.g. ["Michaela Musterfrau <Michaela.Musterfrau@dlr.de>", "Max Mustermann <Max.Mustermann@dlr.de>"]
    developers = []
    
    # * license (string) [recommended]:
    #       please use the "official" licence identifier
    #       you can lookup the identifier on https://spdx.org/licenses/
    #       giving an open source license to a DLR software requires the permission of AL/IL
    #       see also https://wiki.robotic.dlr.de/Open-source_software.
    #       Therefore, the default license is 'proprietary'.
    license = "proprietary"

    # * url (string) [required]
    #       url to internal rmc-github repository
    url = "https://rmc-github.robotic.dlr.de/sewt-ma/Multicam_Dataset"

    # * homepage (string) [recommended]
    #       url to project page for containing more documentation
    homepage = "<url of homepage>"

    # * description (string) [recommended]
    description = "<Description of Testproject here>"

    # * topics (tuple of strings) [optional]:
    #       tags that describe the package
    #       e.g. topics = ("<Put some tag here>", "<here>", "<and here>")
    topics = ()

    # * requires (list) [optional]
    #       package dependencies
    #       use conan syntax, e.g. "OtherLib/2.1@otheruser/testing"
    #
    requires = [
        "boost/[~1.66.0]@3rdparty/stable",
    ]

    # * build_requires (list) [optional]
    #       package build dependencies, i.e.
    #       all packages that are needed to create the package (e.g. dev tools, compilers,
    #       build systems, code analyzers, testing libraries)
    #
    #       Attention: build dependencies will not be passed downstream, i.e.
    #       to generators or packages requiring this one.
    #
    #       use conan syntax, e.g. "OtherLib/2.1@otheruser/testing"
    #
    build_requires = [
        "gtest/1.10.0@3rdparty/stable",
    ]

    # ######################################################### #
    #               build system information
    # ######################################################### #

    # * settings (tuple of strings) [recommended]:
    #       defines ???
    #default: ???
    settings = "os", "compiler", "build_type", "arch"

    # * export_sources (array of filesU/pattern) [required]:
    #       defines all files that are part of the source package
    # default: export everything in the directory that is not excluded by .gitignore
    exports_sources = ["*", "!.gitignore"] + ["!%s" % x for x in tools.Git().excluded_files()]

    #
    # * no_copy_source (boolean) [optional]
    # by default the collected source files (from export_sources) are
    # copied to a source directory in your conan cache
    # and copied again to the build directory.
    # turning on this option, disables the second copy to the build directory.
    # this can be useful if the sources are quite large and the build system supports
    # a build not in source directory.
    #
    # no_copy_source = True

    # * options (dict of <option key : <value range>) [optional]:
    #       defines options that are made public
    #       e.g. options = {"shared": [True, False]}
    # default: empty
    options = {
        # 'fPIC': [False, True],
        # 'shared': [False, True],
    }

    # * default_options (dict of <option key> ; <default value>) [optional]:
    #       specifies the default value for all options
    #       e.g. default_options = {"shared": False}
    # default: empty
    default_options = {
        # 'fPIC': True,
        # 'shared': True,
    }

    # * source_dir (path):
    #       path to sources
    source_dir = "."

    # ######################################################### #
    #            common functions for building and packaging
    # ######################################################### #

    def source(self):
        """source function:
        this function is used for providing and manipulating the source files
        using cissy.conantools, the source is already available in a
        subfolder, defined by 'self.source_dir' (default folder is 'source').
        (see https://docs.conan.io/en/latest/reference/conanfile/methods.html#source)
        """

        pass

    def package_info(self):
        """package_info function:
        Finally, the package_info() method defines
        e.g. that a consumer must link with a certain library when using this package.
        Other information as include or lib paths can be defined as well.
        (see https://docs.conan.io/en/latest/reference/conanfile/methods.html#package-info)
        """

        # auto-set self.cpp_info and self.env_info
        # (see cissy.conantools.autoset_package_info for details)
        # if you want to set individual environments or cpp variables,
        # see https://docs.conan.io/en/latest/reference/conanfile/attributes.html#cpp-info
        # and https://docs.conan.io/en/latest/reference/conanfile/attributes.html#env-info
        #
        from cissy.conantools import autoset_package_info
        autoset_package_info(self)


    # ######################################################### #
    #        cmake specific functions and variables
    # ######################################################### #

    # necessary generators:
    #       cmake_path sets module and prefix path (used at build stage)
    #       (see https://docs.conan.io/en/latest/reference/generators/cmake_paths.html)
    generators = ["cmake_paths", "virtualenv"]

    def init_cmake(self):
        """internal function that configures the cmake builder"""

        # cmake init that tries to use settings.build_type
        cmake = CMake(self)

        # alternatively: cmake init with fixed build_type
        # cmake = CMake(self, build_type="RelWithDebInfo")

        # verbose make
        #
        # cmake.verbose = True

        # setup cmake options from conan settings and options
        # e.g.:

        # use options:
        # cmake.definitions['USE_DOUBLE_PRECISION'] = self.options.double_precision

        # use settings:
        # if self.settings.os == 'Linux':
        #     cmake.definitions['WITH_GTK'] = True

        return cmake

    def build(self):
        """build function:
        the build() should configure and build the project
        (see https://docs.conan.io/en/latest/reference/conanfile/methods.html#build)
        """

        # CMake build
        cmake = self.init_cmake()

        # configure (call to cmake with set definitions)
        cmake_args = []

        # disable package registry
        cmake_args.append("-DCMAKE_FIND_USE_PACKAGE_REGISTRY=FALSE")

        # add cmake_paths (from cmake_paths generator)
        cmake_paths_file = os.path.join(self.build_folder, "conan_paths.cmake")
        if os.path.exists(cmake_paths_file):
            cmake_args.append("-DCMAKE_TOOLCHAIN_FILE={}".format(cmake_paths_file))

        cmake.configure(source_folder=self.source_dir, args=cmake_args)

        # execute 'make all'
        cmake.build()

        # execute other builds,
        # i.e. calls to make with other targets than 'all'
        #
        # cmake.build(target="stuff")

        # execute unit tests if available
        #
        # cmake.test()

    def package(self):
        """package function:
        The package() method should copy artifacts (headers, libs)
        from the build folder to the final package folder.
        (see https://docs.conan.io/en/latest/reference/conanfile/methods.html#package)
        """

        # execute 'make install'
        cmake = self.init_cmake()
        cmake.install()

        # if the package generates cmake config files,
        # this function patches possible abosulte paths
        cmake.patch_config_paths()
