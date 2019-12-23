set ( TargetName 1_TheTyrannyOfTheRocketEquation )

add_executable(
    ${TargetName}
    main.cpp

    TheTyrannyOfTheRocketEquationSolver.h
    TheTyrannyOfTheRocketEquationSolver.cpp
)

target_include_directories( ${TargetName} PRIVATE / )
target_link_libraries( ${TargetName} PRIVATE Helpers )

add_copy_input_command( ${TargetName} Tyranny_Input.txt )
