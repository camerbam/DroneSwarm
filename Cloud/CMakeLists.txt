set(exe_name Cloud)
set(${exe_name}_srcs
  main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder})
