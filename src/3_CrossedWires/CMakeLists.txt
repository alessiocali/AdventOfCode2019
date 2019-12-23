add_executable(
    3_CrossedWires
    main.cpp
    input.txt
)

target_link_libraries( 3_CrossedWires PRIVATE Helpers )
add_copy_input_command( 3_CrossedWires input.txt )