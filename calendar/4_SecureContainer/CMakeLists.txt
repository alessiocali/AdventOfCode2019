set (TargetName 4_SecureContainer)

add_executable(
    ${TargetName}
    main.cpp

    SecureContainerSolver.h
    SecureContainerSolver.cpp
)

target_include_directories( ${TargetName} PRIVATE / )
target_link_libraries( ${TargetName} PRIVATE Helpers Boost::program_options )