#
# TGSInventor
#
if($ENV{DLRRM_HOST_PLATFORM} MATCHES "osl42-x86_64")
    message(FATAL_ERROR "TGSInventor not supported on osl42-86_64")
endif()

set(TGSInventor_VERSION "7.2.0")
set(TGSInventor_ROOT_DIR "/opt/TGSInventor")
set(TGSInventor_INCLUDE_DIR "/opt/TGSInventor/Include")
set(TGSInventor_INCLUDE_DIRS "${TGSInventor_INCLUDE_DIR}")
set(TGSInventor_LIBRARY_DIR "/opt/TGSInventor/lib")
set(TGSInventor_LIBRARY_DIRS "${TGSInventor_LIBRARY_DIR}")

set(TGSInventor_LIBS InventorQt4 Inventor)

foreach(_TGSInventor_lib ${TGSInventor_LIBS})
    unset(_tmp_tgs CACHE)
    find_library(_tmp_tgs ${_TGSInventor_lib} PATHS ${TGSInventor_LIBRARY_DIRS})
    list(APPEND TGSInventor_LIBRARIES ${_tmp_tgs})
endforeach(_TGSInventor_lib)

message(STATUS "TGSInventor ${TGSInventor_VERSION} from ${TGSInventor_ROOT_DIR}")
mark_as_advanced(TGSInventor_INCLUDE_DIR TGSInventor_INCLUDE_DIRS TGSInventor_LIBRARY_DIR TGSInventor_LIBRARY_DIRS TGSInventor_LIBRARIES)
