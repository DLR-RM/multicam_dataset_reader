
if(NOT MulticamDatasetReader_Boost_LOADED)
    message(STATUS "> searching for cmake package 'Boost'")
    find_package(Boost REQUIRED COMPONENTS filesystem)
    set(MulticamDatasetReader_Boost_LOADED True)
endif()
        