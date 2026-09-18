cmake_minimum_required(VERSION 3.27)

project(vector_example LANGUAGES CXX)

add_library(vector STATIC vector.cpp)
target_include_directories(vector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(vector PUBLIC cxx_std_20)

add_executable(vector_demo main.cpp)
target_link_libraries(vector_demo PRIVATE vector)

include(CTest)
if(BUILD_TESTING)
    add_executable(vector_test test.cpp)
    target_link_libraries(vector_test PRIVATE vector)
    add_test(NAME vector_test COMMAND vector_test)
endif()
