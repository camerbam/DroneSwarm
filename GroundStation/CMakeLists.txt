set(exe_name GroundStation)
set(${exe_name}_srcs
  main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
xpTargetExtern(${lib_name} PRIVATE boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
