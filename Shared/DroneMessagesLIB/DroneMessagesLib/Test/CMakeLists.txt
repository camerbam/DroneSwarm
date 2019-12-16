set(exe_name DroneMessagesTest)
set(${exe_name}_srcs
  ${exe_name}Main.cpp
  BackMessageTest.cpp
  BatteryMessageTest.cpp
  ClockwiseMessageTest.cpp
  CommandMessageTest.cpp
  CounterClockwiseMessageTest.cpp
  DownMessageTest.cpp
  DroneStatusMessageTest.cpp
  FlipMessageTest.cpp
  ForwardMessageTest.cpp
  GoMessageTest.cpp
  LandMessageTest.cpp
  LeftMessageTest.cpp
  MessageFactoryTest.cpp
  RightMessageTest.cpp
  SpeedMessageTest.cpp
  TakeoffMessageTest.cpp
  TimeMessageTest.cpp
  UpMessageTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC DroneMessagesLib)
xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder}/UnitTest)
add_test(NAME ${exe_name} COMMAND ${exe_name})
xpSourceListAppend("${${exe_name}_srcs}")
