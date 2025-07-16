set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O2 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

add_library(blaze STATIC fuzzy_matcher.cpp highlighter.cpp config.cpp io.cpp)
target_include_directories(blaze PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(blaze_main main.cpp)
target_link_libraries(blaze_main PRIVATE blaze)
