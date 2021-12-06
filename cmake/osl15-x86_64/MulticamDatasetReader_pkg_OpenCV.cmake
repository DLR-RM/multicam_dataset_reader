
if(NOT MulticamDatasetReader_OpenCV_LOADED)
    message(STATUS "> searching for cmake package 'OpenCV'")
    find_package(OpenCV REQUIRED COMPONENTS core imgcodecs highgui)
    set(MulticamDatasetReader_OpenCV_LOADED True)
endif()
        