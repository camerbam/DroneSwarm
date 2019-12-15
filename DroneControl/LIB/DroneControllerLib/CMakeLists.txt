set(lib_name DroneControllerLib)
set(${lib_name}_srcs
  DroneController.cpp
  DroneController.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC UDP)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/DroneControl/LIB)
xpTargetExtern(${lib_name} PRIVATE boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
