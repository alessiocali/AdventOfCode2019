set ( TargetName 5_SunnyWithAChanceOfAsteroids )

add_executable(
    ${TargetName}
    main.cpp

    SunnyWithAChanceOfAsteroidsSolver.h
    SunnyWithAChanceOfAsteroidsSolver.cpp
)

target_link_libraries( ${TargetName} PRIVATE Boost::program_options Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} Sunny_Input.txt )