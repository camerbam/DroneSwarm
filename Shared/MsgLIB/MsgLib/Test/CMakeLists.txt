set(exe_name MsgTest)
set(${exe_name}_srcs
  BaseMsgTest.cpp
  EncryptedMsgTest.cpp
  FinishMsgTest.cpp
  FinishRspTest.cpp
  FlightPathMsgTest.cpp
  FlightPathRspTest.cpp
  HitTargetMsgTest.cpp
  HitTargetRspTest.cpp
  MsgTest.cpp
  PingMsgTest.cpp
  PingRspTest.cpp
  ReadyMsgTest.cpp
  ReadyRspTest.cpp
  ReadyRspRspTest.cpp
  StringMsgTest.cpp
  TargetTest.cpp
  ZConfigMsgTest.cpp
  ZConfigRspTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC Msg)
xpTargetExtern(${exe_name} PUBLIC boost)
target_link_libraries(${exe_name} PUBLIC ${Protobuf_LIBRARIES})
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder}/UnitTest)
add_test(NAME ${exe_name} COMMAND ${exe_name})
xpSourceListAppend("${${exe_name}_srcs}")
