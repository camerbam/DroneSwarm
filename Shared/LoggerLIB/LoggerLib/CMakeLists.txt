set(lib_name Logger)
set(${lib_name}_srcs
  main.cpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
