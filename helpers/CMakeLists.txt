add_library( Helpers
    include/CommonDefines.h
    include/MathHelpers.h
    include/ProblemSolver.h

    include/PermutationGenerator.h

    include/IntcodeProgram.h
    src/IntcodeProgram.cpp

    include/SimpleControllableView.h
    src/SimpleControllableView.cpp

    include/CommonHelpers.h
    src/CommonHelpers.cpp
)

target_include_directories( Helpers
    PUBLIC    
    include/
)

target_compile_features( Helpers
    PUBLIC
    cxx_std_17
)

target_link_libraries( Helpers 
    PUBLIC 
    sfml-system 
    sfml-window 
    sfml-graphics 
    
    Boost::boost

    PRIVATE
    Boost::program_options
)