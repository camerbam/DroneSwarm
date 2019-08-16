set(lib_name logger)
set(${lib_name}_srcs
  Logger.cpp
  Logger.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PRIVATE acq)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/LoggerLIB)
xpTargetExtern(${lib_name} PUBLIC boost)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
