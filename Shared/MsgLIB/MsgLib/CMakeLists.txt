set(lib_name Msg)
set(${lib_name}_srcs
  BaseMsg.cpp
  BaseMsg.hpp
  FlightPathMsg.cpp
  FlightPathMsg.hpp
  FlightPathRsp.cpp
  FlightPathRsp.hpp
  MsgTypes.cpp
  MsgTypes.hpp
  Point.cpp
  Point.hpp
  SerializeDeserialize.hpp
  StringMsg.cpp
  StringMsg.hpp
  TargetMsg.cpp
  TargetMsg.hpp
  TargetRsp.cpp
  TargetRsp.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC json Proto xml)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/MsgLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
xpSourceListAppend("${${lib_name}_srcs}")
