add_library( Helpers
    include/MathHelpers.h

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