
if(NOT MulticamDatasetReader_gdal_LOADED)
    message(STATUS "> searching for cmake package 'gdal'")
    find_package(gdal REQUIRED)
    set(MulticamDatasetReader_gdal_LOADED True)
endif()
        