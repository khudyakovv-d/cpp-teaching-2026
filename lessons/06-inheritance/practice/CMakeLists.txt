cmake_minimum_required(VERSION 3.20)
project(inheritance_demo LANGUAGES CXX)

add_executable(inheritance_demo main.cpp shapes.cpp)
target_compile_features(inheritance_demo PRIVATE cxx_std_20)
