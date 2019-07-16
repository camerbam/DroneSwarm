set(lib_name TCP)
set(${lib_name}_srcs
  TCPClient.cpp
  TCPClient.hpp
  TCPServer.cpp
  TCPServer.hpp
  TCPTools.cpp
  TCPTools.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC ACQ Msg)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/TCPLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
