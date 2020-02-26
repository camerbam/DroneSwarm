set(lib_name StatusManagerLib)
set(${lib_name}_srcs
  StatusManager.cpp
  StatusManager.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC udp registry logger)
target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
xpTargetExtern(${lib_name} PUBLIC boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
xpSourceListAppend(${${lib_name}_srcs})
