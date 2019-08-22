project(Shared)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################
# No Dependency
add_subdirectory(ACQLIB/ACQLib)
add_subdirectory(LoggerLIB/LoggerLib)
add_subdirectory(ProtoLIB/ProtoLib)
add_subdirectory(JSONLIB/JSONLib)
add_subdirectory(XMLLIB/XMLLib)
##################################
# Second Layer Dependency
add_subdirectory(MsgLIB/MsgLib)
add_subdirectory(UDPLIB/UDPLib)
##################################
# Third Layer Dependency
add_subdirectory(TCPLIB/TCPLib)
