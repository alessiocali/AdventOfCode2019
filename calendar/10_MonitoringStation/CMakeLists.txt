set ( TargetName 10_MonitoringStation )

add_executable(
    ${TargetName}
    main.cpp

    MonitoringStationSolver.h
    MonitoringStationSolver.cpp
)

target_link_libraries( ${TargetName} PRIVATE Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} MonitoringStation_Input.txt )
add_copy_input_command( ${TargetName} MonitoringStation_TestSimple.txt )
add_copy_input_command( ${TargetName} MonitoringStation_Test1.txt )
add_copy_input_command( ${TargetName} MonitoringStation_Test3.txt )