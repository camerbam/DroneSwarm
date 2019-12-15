set(exe_name DroneSimulator)
set(folder ${exe_name})
add_subdirectory(LIB/${exe_name}Lib)
add_subdirectory(Test)
set(${exe_name}_srcs
  ${DroneSimulator}Main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PUBLIC ${exe_name}Lib)
# xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${exe_name})
