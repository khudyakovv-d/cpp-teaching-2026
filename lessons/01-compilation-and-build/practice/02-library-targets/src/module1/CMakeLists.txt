add_library(module1 STATIC module1.cpp module1.h)

target_compile_features(module1 PRIVATE cxx_std_20)

target_include_directories(
    module1
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
