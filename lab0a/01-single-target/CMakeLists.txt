cmake_minimum_required(VERSION 3.27)

project(lab0a LANGUAGES CXX)

add_executable(
    lab0a
    main.cpp
    module1.cpp
    module2.cpp
)

target_compile_features(lab0a PRIVATE cxx_std_20)
