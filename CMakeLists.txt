cmake_minimum_required(VERSION 3.28)
project(Rodent)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDE "include/*.hpp" "include/*.h")

include_directories(include)

add_executable(Rodent
        main.cpp
        ${SOURCES}
        ${INCLUDE}
)

