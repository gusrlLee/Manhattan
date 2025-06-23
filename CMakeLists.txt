cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

project(Manhattan)

add_subdirectory(ThirdParty)

add_executable(${PROJECT_NAME} 
    main.cpp
    
    Vector3.hpp
    Color.hpp
    Image.hpp
    Image.cpp

    Camera.hpp

    Interval.hpp
    Interval.cpp

    RNG.hpp
    RNG.cpp

)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/ThirdParty)
