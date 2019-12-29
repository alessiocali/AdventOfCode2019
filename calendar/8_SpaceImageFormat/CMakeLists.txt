set ( TargetName 8_SpaceImageFormat )

add_executable(
    ${TargetName}
    main.cpp

    SpaceImageFormatSolver.h
    SpaceImageFormatSolver.cpp
)

target_link_libraries( ${TargetName} PRIVATE Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} SpaceImage_Input.txt )