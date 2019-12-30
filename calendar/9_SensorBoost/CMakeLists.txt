set ( TargetName 9_SensorBoost )

add_executable(
    ${TargetName}
    main.cpp

    SensorBoostSolver.h
    SensorBoostSolver.cpp
)

target_link_libraries( ${TargetName} PRIVATE Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} Boost_Input.txt )
add_copy_input_command( ${TargetName} Test0_Input.txt )
add_copy_input_command( ${TargetName} Test1_Input.txt )
add_copy_input_command( ${TargetName} Test2_Input.txt )