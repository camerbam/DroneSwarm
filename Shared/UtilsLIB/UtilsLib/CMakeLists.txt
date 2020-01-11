set(lib_name utils)
set(${lib_name}_srcs
  Utils.cpp
  Utils.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/UtilsLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
xpSourceListAppend("${${lib_name}_srcs}")
