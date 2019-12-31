cmake_minimum_required(VERSION 3.14)

project( AdventOfCode2019 
    DESCRIPTION
    "A set of solutions programmed in C++ for the Advent Of Code 2019"
    LANGUAGES CXX
)

macro (add_copy_input_command TargetName InputFile)
    add_custom_command(
        TARGET ${TargetName} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/calendar/${TargetName}/${InputFile}
        ${CMAKE_CURRENT_BINARY_DIR}/${InputFile}
    )
endmacro()

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Only build static libs.")

# Add external dependencies
add_subdirectory(extern)
add_subdirectory(helpers)

# Add anti regression tests
add_subdirectory(tests)

# Executables for each day
add_subdirectory(calendar/1_TheTyrannyOfTheRocketEquation)
add_subdirectory(calendar/2_1202ProgramAlarm)
add_subdirectory(calendar/3_CrossedWires)
add_subdirectory(calendar/4_SecureContainer)
add_subdirectory(calendar/5_SunnyWithAChanceOfAsteroids)
add_subdirectory(calendar/6_UniversalOrbitMap)
add_subdirectory(calendar/7_AmplificationCircuit)
add_subdirectory(calendar/8_SpaceImageFormat)
add_subdirectory(calendar/9_SensorBoost)
add_subdirectory(calendar/10_MonitoringStation)