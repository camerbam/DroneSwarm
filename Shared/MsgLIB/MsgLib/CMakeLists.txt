set(lib_name Msg)
set(${lib_name}_srcs
  BaseMsg.cpp
  BaseMsg.hpp
  EncryptedMsg.cpp
  EncryptedMsg.hpp
  FinishMsg.cpp
  FinishMsg.hpp
  FinishRsp.cpp
  FinishRsp.hpp
  FlightPathMsg.cpp
  FlightPathMsg.hpp
  FlightPathRsp.cpp
  FlightPathRsp.hpp
  HitTargetMsg.cpp
  HitTargetMsg.hpp
  HitTargetRsp.cpp
  HitTargetRsp.hpp
  MsgTypes.cpp
  MsgTypes.hpp
  PingMsg.cpp
  PingMsg.hpp
  PingRsp.cpp
  PingRsp.hpp
  SerializeDeserialize.hpp
  ReadyMsg.cpp
  ReadyMsg.hpp
  ReadyRsp.cpp
  ReadyRsp.hpp
  ReadyRspRsp.cpp
  ReadyRspRsp.hpp
  StringMsg.cpp
  StringMsg.hpp
  TargetMsg.cpp
  TargetMsg.hpp
  ZConfigMsg.cpp
  ZConfigMsg.hpp
  ZConfigRsp.cpp
  ZConfigRsp.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC json Proto xml utils registry)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/MsgLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
xpSourceListAppend("${${lib_name}_srcs}")
