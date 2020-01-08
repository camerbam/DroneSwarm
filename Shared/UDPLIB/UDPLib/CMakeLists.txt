set(lib_name udp)
set(${lib_name}_srcs
  UDPCommunicator.cpp
  UDPCommunicator.hpp
  UDPCommunicatorReliable.cpp
  UDPCommunicatorReliable.hpp
  Response.cpp
  Response.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/UDPLIB)
xpTargetExtern(${lib_name} PUBLIC boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
xpSourceListAppend(${${lib_name}_srcs})
