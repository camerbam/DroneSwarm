set(exe_name DroneSimulatorTest)
set(${exe_name}_srcs
  DroneConfigurationTest.cpp
  DroneLocationTest.cpp
  DroneSimulatorIdleStateTest.cpp
  DroneSimulatorStateChangesTest.cpp
  DroneSimulatorTest.cpp
  DroneSimulatorTestMain.cpp
  DroneSimulatorStateImplTest.cpp
  LocationUpdaterTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC DroneSimulatorLib)
xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER UnitTests)
add_test(NAME ${exe_name} COMMAND ${exe_name})
xpSourceListAppend(${${exe_name}_srcs})
