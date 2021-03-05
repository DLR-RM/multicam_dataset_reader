message(STATUS "Testing enabled")
find_package(Boost 1.58 REQUIRED COMPONENTS unit_test_framework filesystem system)

add_executable(test_reader test_reader.cc)
target_link_libraries(test_reader ${Boost_LIBRARIES})
add_test(tests test_reader)