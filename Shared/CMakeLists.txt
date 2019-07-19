project(Shared)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################
# No Dependency
add_subdirectory(ACQLIB/ACQLib)
add_subdirectory(JSONLIB/JSONLib)
add_subdirectory(LoggerLIB/LoggerLib)
add_subdirectory(ProtoLIB/ProtoLib)
##################################
# Second Layer Dependency
add_subdirectory(MsgLIB/MsgLib)
##################################
# Third Layer Dependency
add_subdirectory(TCPLIB/TCPLib)
