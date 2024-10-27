cmake_minimum_required(VERSION 3.29)
project(Rodent)

set(CMAKE_CXX_STANDARD 26)

file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDE "include/*.hpp" "include/*.h")

include_directories(include)

add_executable(Rodent
        main.cpp
        ${SOURCES}
        ${INCLUDE}
)

