set(exe_name UDPTest)
set(${exe_name}_srcs
  ResponseTest.cpp
  UDPCommunicatorReliableTest.cpp
  UDPCommunicatorTest.cpp
  UDPTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC udp)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder}/UnitTest)
add_test(NAME ${exe_name} COMMAND ${exe_name})
