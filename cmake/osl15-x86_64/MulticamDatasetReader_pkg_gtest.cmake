
if(NOT MulticamDatasetReader_gtest_LOADED)
    message(STATUS "> searching for cmake package 'gtest'")
    find_package(gtest REQUIRED)
    set(MulticamDatasetReader_gtest_LOADED True)
endif()
        