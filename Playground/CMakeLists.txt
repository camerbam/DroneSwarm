set(exe_name Playground)
set(${exe_name}_srcs
  main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC udp)
xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})
install(TARGETS ${exe_name} DESTINATION dist)
xpSourceListAppend(${${exe_name}_srcs})
