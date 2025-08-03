cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
project(Manhattan)

find_package(glm REQUIRED)

add_executable(pt pt/pt.cpp pt/pt.hpp)
target_link_libraries(pt glm::glm-header-only)
