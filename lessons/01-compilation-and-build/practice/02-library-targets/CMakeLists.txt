cmake_minimum_required(VERSION 3.27)

project(lab0a LANGUAGES CXX)

add_subdirectory(src/module1)

add_executable(
    lab0a
    main.cpp
    src/module2/module2.cpp
    src/module2/module2.h
)

target_compile_features(lab0a PRIVATE cxx_std_20)

target_include_directories(
    lab0a
    PRIVATE
        src/module2
)

target_link_libraries(lab0a PRIVATE module1)
