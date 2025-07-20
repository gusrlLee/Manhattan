cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(Manhattan)
add_subdirectory(ThirdParty)

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME}
    window.h
    main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/ThirdParty/metal-cpp
)

target_link_libraries(${PROJECT_NAME}
    METAL_CPP_LIB
    SDL3::SDL3
)
