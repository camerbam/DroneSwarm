set(lib_name Proto)
set(${lib_name}_srcs
  StringMsg.proto
  )
source_group("" FILES ${${lib_name}_srcs})
list(APPEND ${lib_name}_proto_srcs ${${lib_name}_srcs})
PROTOBUF_GENERATE_CPP(${lib_name}_cpp ${lib_name}_hpp ${lib_name}_proto_srcs})
add_library(${lib_name} ${${lib_name}_srcs})
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
