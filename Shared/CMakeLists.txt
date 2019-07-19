project(Shared)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################
add_subdirectory(LoggerLIB/LoggerLib)
add_subdirectory(ACQLIB/ACQLib)
add_subdirectory(JSONLIB/JSONLib)
