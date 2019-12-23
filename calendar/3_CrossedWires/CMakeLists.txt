set ( TargetName 3_CrossedWires )

add_executable(
    ${TargetName}
    main.cpp
    
    CrossedWiresSolver.cpp
    CrossedWiresSolver.h
)

target_link_libraries( ${TargetName} PRIVATE Helpers )
target_include_directories( ${TargetName} PRIVATE / )

add_copy_input_command( ${TargetName} Wires_Input.txt )