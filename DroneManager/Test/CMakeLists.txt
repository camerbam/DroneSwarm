set(exe_name DroneManagerTest)
set(${exe_name}_srcs
  DroneControllerCheckMsgToSend.cpp
  DroneControllerMainTest.cpp
  DroneControllerMessagesToString.cpp
  DroneControllerStateChangesTest.cpp
  DroneControllerTest.cpp
  DroneManagerTest.cpp
  DroneStateTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PRIVATE DroneManagerLib DroneSimulatorLib utils monitorlogger)
set_property(TARGET ${exe_name} PROPERTY FOLDER UnitTests)
add_test(NAME ${exe_name} COMMAND ${exe_name})
xpSourceListAppend(${${exe_name}_srcs})
