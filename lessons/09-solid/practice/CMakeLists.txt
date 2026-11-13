cmake_minimum_required(VERSION 3.20)
project(solid_demo LANGUAGES CXX)

add_executable(solid_demo main.cpp srp.cpp ocp.cpp lsp.cpp isp.cpp dip.cpp)
target_compile_features(solid_demo PRIVATE cxx_std_20)
