
add_library(blaze STATIC fuzzy_matcher.cpp highlighter.cpp config.cpp io.cpp)
target_include_directories(blaze PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(blaze_main main.cpp)
target_link_libraries(blaze_main PRIVATE blaze)
