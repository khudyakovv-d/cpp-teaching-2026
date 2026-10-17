cmake_minimum_required(VERSION 3.27)
project(vector_gtest_example LANGUAGES CXX)

set(VECTOR_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../02-classes-and-vector/practice")
add_library(vector STATIC "${VECTOR_SOURCE_DIR}/vector.cpp")
target_include_directories(vector PUBLIC "${VECTOR_SOURCE_DIR}")
target_compile_features(vector PUBLIC cxx_std_20)

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(vector_tests test.cpp)
    target_link_libraries(vector_tests PRIVATE vector GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(vector_tests)
endif()
