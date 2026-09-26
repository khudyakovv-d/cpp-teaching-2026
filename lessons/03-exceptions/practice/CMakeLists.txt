cmake_minimum_required(VERSION 3.20)
project(exceptions_demo LANGUAGES CXX)

add_executable(exceptions_demo
    main.cpp
    ../../02-classes-and-vector/practice/vector.cpp
)
target_include_directories(exceptions_demo PRIVATE ../../02-classes-and-vector/practice)
target_compile_features(exceptions_demo PRIVATE cxx_std_20)
