
if(NOT PkgConfig_FOUND)
    message(STATUS "> loading pkg-config support")
    find_package(PkgConfig REQUIRED)
endif()

if(NOT MulticamDatasetReader_tclap_LOADED)
    message(STATUS "> searching for pkgconfig package 'tclap'")
    pkg_check_modules(tclap REQUIRED IMPORTED_TARGET tclap)
    set(MulticamDatasetReader_tclap_LOADED True)
endif()
    