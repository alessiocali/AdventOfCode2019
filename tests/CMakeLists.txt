set (CalendarDir ${CMAKE_SOURCE_DIR}/calendar)

add_executable(
    Tests
    catch2_main.cpp

    ${CalendarDir}/1_TheTyrannyOfTheRocketEquation/TheTyrannyOfTheRocketEquationSolver.h
    ${CalendarDir}/1_TheTyrannyOfTheRocketEquation/TheTyrannyOfTheRocketEquationSolver.cpp

    ${CalendarDir}/2_1202ProgramAlarm/1202ProgramAlarmSolver.h
    ${CalendarDir}/2_1202ProgramAlarm/1202ProgramAlarmSolver.cpp

    ${CalendarDir}/3_CrossedWires/CrossedWiresSolver.h
    ${CalendarDir}/3_CrossedWires/CrossedWiresSolver.cpp

    ${CalendarDir}/4_SecureContainer/SecureContainerSolver.h
    ${CalendarDir}/4_SecureContainer/SecureContainerSolver.cpp
)

target_link_libraries(
    Tests

    PRIVATE
    Catch2::Catch2
    Helpers
)

target_include_directories(
    Tests
    
    PRIVATE
    ${CalendarDir}/1_TheTyrannyOfTheRocketEquation
    ${CalendarDir}/2_1202ProgramAlarm
    ${CalendarDir}/3_CrossedWires
    ${CalendarDir}/4_SecureContainer
)

file ( COPY ${CalendarDir}/1_TheTyrannyOfTheRocketEquation/Tyranny_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/2_1202ProgramAlarm/1202_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/3_CrossedWires/Wires_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
# No input file for 4_SecureContainer