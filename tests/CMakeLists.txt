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

    ${CalendarDir}/5_SunnyWithAChanceOfAsteroids/SunnyWithAChanceOfAsteroidsSolver.h
    ${CalendarDir}/5_SunnyWithAChanceOfAsteroids/SunnyWithAChanceOfAsteroidsSolver.cpp

    ${CalendarDir}/6_UniversalOrbitMap/UniversalOrbitMapSolver.h
    ${CalendarDir}/6_UniversalOrbitMap/UniversalOrbitMapSolver.cpp

    ${CalendarDir}/7_AmplificationCircuit/AmplificationCircuitSolver.h
    ${CalendarDir}/7_AmplificationCircuit/AmplificationCircuitSolver.cpp

    ${CalendarDir}/8_SpaceImageFormat/SpaceImageFormatSolver.h
    ${CalendarDir}/8_SpaceImageFormat/SpaceImageFormatSolver.cpp

    ${CalendarDir}/9_SensorBoost/SensorBoostSolver.h
    ${CalendarDir}/9_SensorBoost/SensorBoostSolver.cpp

    ${CalendarDir}/10_MonitoringStation/MonitoringStationSolver.h
    ${CalendarDir}/10_MonitoringStation/MonitoringStationSolver.cpp
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
    ${CalendarDir}/5_SunnyWithAChanceOfAsteroids
    ${CalendarDir}/6_UniversalOrbitMap
    ${CalendarDir}/7_AmplificationCircuit
    ${CalendarDir}/8_SpaceImageFormat
    ${CalendarDir}/9_SensorBoost
    ${CalendarDir}/10_MonitoringStation
)

file ( COPY ${CalendarDir}/1_TheTyrannyOfTheRocketEquation/Tyranny_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/2_1202ProgramAlarm/1202_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/3_CrossedWires/Wires_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
# No input file for 4_SecureContainer
file ( COPY ${CalendarDir}/5_SunnyWithAChanceOfAsteroids/Sunny_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/6_UniversalOrbitMap/Orbit_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/7_AmplificationCircuit/Amplification_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/8_SpaceImageFormat/SpaceImage_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/9_SensorBoost/Boost_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )
file ( COPY ${CalendarDir}/10_MonitoringStation/MonitoringStation_Input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/inputs/ )