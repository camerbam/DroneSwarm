set(lib_name Msg)
set(${lib_name}_srcs
  BaseMsg.cpp
  BaseMsg.hpp
  MsgTypes.hpp
  StringMsg.cpp
  StringMsg.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_link_libraries(${lib_name} PRIVATE json Proto xml)
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/MsgLIB)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
