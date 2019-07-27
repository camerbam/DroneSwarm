project(Shared)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################
add_subdirectory(ACQLIB/ACQLib)
add_subdirectory(LoggerLIB/LoggerLib)
add_subdirectory(ProtoLIB/ProtoLib)
add_subdirectory(JSONLIB/JSONLib)
add_subdirectory(XMLLIB/XMLLib)
