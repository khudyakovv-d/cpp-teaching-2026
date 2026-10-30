cmake_minimum_required(VERSION 3.20)
project(multiple_inheritance_and_casts_demo LANGUAGES CXX)

add_executable(casts_demo main.cpp)
target_compile_features(casts_demo PRIVATE cxx_std_20)
