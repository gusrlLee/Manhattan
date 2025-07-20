cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

add_library(METAL_CPP_LIB 
    metal_definition.cpp
)

target_link_libraries(METAL_CPP_LIB
    "-framework Metal"
    "-framework QuartzCore"
    "-framework Foundation"
)