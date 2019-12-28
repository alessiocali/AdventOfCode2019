set ( TargetName 7_AmplificationCircuit )

add_executable(
    ${TargetName}
    main.cpp

    AmplificationCircuitSolver.h
    AmplificationCircuitSolver.cpp
)

target_link_libraries( ${TargetName} PRIVATE Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} Amplification_Input.txt )