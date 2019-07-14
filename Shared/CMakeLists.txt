project(Shared)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################
# No Dependency
add_subdirectory(LoggerLIB/LoggerLib)
add_subdirectory(ACQLIB/ACQLib)
add_subdirectory(MsgLIB/MsgLib)
##################################
# Second Layer Dependency
add_subdirectory(TCPLIB/TCPLib)
