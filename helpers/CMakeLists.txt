add_library( Helpers
    include/CommonDefines.h
    include/MathHelpers.h
    include/ProblemSolver.h
    include/IntcodeProgram.h

    include/SimpleControllableView.h
    src/SimpleControllableView.cpp
)

target_include_directories( Helpers
    PUBLIC    
    include/
    
    PRIVATE
    src/
)

target_link_libraries( Helpers PUBLIC sfml-system sfml-window sfml-graphics )