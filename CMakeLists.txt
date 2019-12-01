cmake_minimum_required(VERSION 3.14)

project( AdventOfCode2019 
    DESCRIPTION
    "A set of solutions programmed in C++ for the Advent Of Code 2019"
    LANGUAGES CXX
)

# Executables for each day
add_executable(
    1_TheTyrannyOfTheRocketEquation
    src/1/main.cpp
    src/1/input.txt
)

add_custom_command(
    TARGET 1_TheTyrannyOfTheRocketEquation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/1/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)