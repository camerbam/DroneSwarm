set(exe_name DroneSimulator)
set(folder ${exe_name})
add_subdirectory(LIB)
add_subdirectory(Test)
set(${exe_name}_srcs
  ${DroneSimulator}Main.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PRIVATE ${exe_name}LIB)
# xpTargetExtern(${exe_name} PRIVATE boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${exe_name})
