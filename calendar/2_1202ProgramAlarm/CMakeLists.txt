set ( TargetName 2_1202ProgramAlarm )

add_executable(
    ${TargetName}
    main.cpp
    
    1202ProgramAlarmSolver.h
    1202ProgramAlarmSolver.cpp
)

target_include_directories( ${TargetName} PRIVATE / )
target_link_libraries( ${TargetName} PRIVATE Helpers )

add_copy_input_command( ${TargetName} 1202_Input.txt )