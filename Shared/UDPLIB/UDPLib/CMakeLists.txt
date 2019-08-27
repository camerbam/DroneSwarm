set(lib_name UDP)
set(${lib_name}_srcs
  UDPSender.cpp
  UDPSender.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC acq)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/UDPLIB)
xpTargetExtern(${lib_name} PRIVATE boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
