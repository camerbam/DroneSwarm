set(lib_name RefereeLib)
set(${lib_name}_srcs
  GameManager.cpp
  GameManager.hpp
  Graph.cpp
  Graph.hpp
  PretestController.cpp
  PretestController.hpp
  RefereeController.cpp
  RefereeController.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PUBLIC monitorlogger registry utils)
target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
xpTargetExtern(${lib_name} PUBLIC boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
xpSourceListAppend(${${lib_name}_srcs})
