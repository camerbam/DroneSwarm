set(lib_name json)
set(${lib_name}_srcs
  Deserialize.cpp
  Deserialize.hpp
  Serialize.cpp
  Serialize.hpp
  )
source_group("" FILES ${${lib_name}_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
target_include_directories(${lib_name} PUBLIC ${CMAKE_SOURCE_DIR}/Shared/JSONLIB)
xpTargetExtern(${lib_name} PUBLIC rapidjson)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)