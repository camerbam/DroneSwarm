set(exe_name MockGroundStation)
set(${exe_name}_srcs
  main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC TCP)
xpTargetExtern(${lib_name} PRIVATE boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
install(TARGETS ${exe_name} DESTINATION dist)
xpSourceListAppend(${${exe_name}_srcs})
