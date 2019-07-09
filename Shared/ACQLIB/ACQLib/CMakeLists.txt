set(lib_name ACQ)
set(${lib_name}_srcs
  ACQ.cpp
  ACQ.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
xpTargetExtern(${lib_name} PUBLIC boost)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/ACQLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
