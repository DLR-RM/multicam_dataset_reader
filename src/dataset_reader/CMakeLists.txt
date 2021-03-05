configure_file(Version.h.in ${PROJECT_BINARY_DIR}/Version.h)

set(LIB_HEADERS
        DatasetReader.h
        Filesystem.h
        )

set(LIB_SOURCES
        DatasetReader.cc
        )

add_library(multicam_dataset_reader ${LIB_SOURCES} ${LIB_HEADERS})