
if(NOT MulticamDatasetReader_Eigen_LOADED)
    message(STATUS "> searching for cmake package 'Eigen'")
    find_package(Eigen REQUIRED)
    set(MulticamDatasetReader_Eigen_LOADED True)
endif()
        