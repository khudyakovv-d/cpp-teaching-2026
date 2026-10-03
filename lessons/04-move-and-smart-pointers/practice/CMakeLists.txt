cmake_minimum_required(VERSION 3.20)
project(move_and_smart_pointers_demo LANGUAGES CXX)

add_executable(move_and_smart_pointers_demo main.cpp vector.cpp)
target_compile_features(move_and_smart_pointers_demo PRIVATE cxx_std_20)
