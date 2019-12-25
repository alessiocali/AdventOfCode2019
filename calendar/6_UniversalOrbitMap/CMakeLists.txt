set ( TargetName 6_UniversalOrbitMap )

add_executable(
    ${TargetName}
    main.cpp

    UniversalOrbitMapSolver.h
    UniversalOrbitMapSolver.cpp
)

target_link_libraries( ${TargetName} PRIVATE Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} Orbit_Input.txt )