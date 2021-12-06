
if(NOT PkgConfig_FOUND)
    message(STATUS "> loading pkg-config support")
    find_package(PkgConfig REQUIRED)
endif()

if(NOT MulticamDatasetReader_gdal_LOADED)
    message(STATUS "> searching for pkgconfig package 'gdal'")
    pkg_check_modules(gdal REQUIRED IMPORTED_TARGET gdal)
    set(MulticamDatasetReader_gdal_LOADED True)
endif()
    