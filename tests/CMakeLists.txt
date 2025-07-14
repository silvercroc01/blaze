
add_executable(blaze_tests fuzz_test.cpp)
target_link_libraries(blaze_tests PRIVATE blaze)

# external library doctest is in vendor/
target_include_directories(blaze_tests PRIVATE ${CMAKE_SOURCE_DIR}/vendor)

enable_testing()
add_test(NAME BlazeTests COMMAND blaze_tests)
