set(exe_name TCPClient)
set(${exe_name}_srcs
  main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PRIVATE TCP)
install(TARGETS ${exe_name} DESTINATION dist)
xpTargetExtern(${lib_name} PRIVATE boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
