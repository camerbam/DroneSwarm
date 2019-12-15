set(exe_name DroneSimulatorTest)
set(${exe_name}_srcs
  DroneSimulatorTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PRIVATE DroneSimulatorLib)
xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})
add_test(NAME ${exe_name} COMMAND ${exe_name})