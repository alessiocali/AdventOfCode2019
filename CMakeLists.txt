cmake_minimum_required(VERSION 3.14)

project( AdventOfCode2019 
    DESCRIPTION
    "A set of solutions programmed in C++ for the Advent Of Code 2019"
    LANGUAGES CXX
)

macro (add_copy_input_command TargetName TargetIdx InputFile)
    add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/${TargetIdx}/${InputFile}
        ${CMAKE_CURRENT_BINARY_DIR}/${InputFile}
    )
endmacro()

macro (add_day_executable DayName DayIdx)
    set ( ExecutableName ${DayIdx}_${DayName} )
    add_executable( 
        ${ExecutableName}
        src/${DayIdx}/main.cpp
        src/${DayIdx}/input.txt
    )
    add_copy_input_command( ${DayIdx}_${DayName} ${DayIdx} input.txt )
    target_link_libraries( ${ExecutableName} PRIVATE Helpers )
endmacro()

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Only build static libs.")

# Add external dependencies
add_subdirectory(extern)
add_subdirectory(helpers)

# Executables for each day
add_day_executable( TheTyrannyOfTheRocketEquation 1 )
add_day_executable( 1202ProgramAlarm 2 )
add_day_executable( CrossedWires 3 )