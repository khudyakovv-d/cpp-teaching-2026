cmake_minimum_required(VERSION 3.20)
project(factories_demo LANGUAGES CXX)

add_executable(factories_demo main.cpp)
target_compile_features(factories_demo PRIVATE cxx_std_20)
