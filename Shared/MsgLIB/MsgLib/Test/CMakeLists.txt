set(exe_name MsgTest)
set(${exe_name}_srcs
  BaseMsgTest.cpp
  MsgTest.cpp
  StringMsgTest.cpp
  )
source_group("" FILES ${${exe_name}_srcs})
add_executable(${exe_name} ${${exe_name}_srcs})
target_link_libraries(${exe_name} PRIVATE Msg)
xpTargetExtern(${exe_name} PUBLIC boost)
set_property(TARGET ${exe_name} PROPERTY FOLDER ${folder}/UnitTest)
add_test(NAME ${exe_name} COMMAND ${exe_name})
xpSourceListAppend("${${exe_name}_srcs}")
