project(Shared)
set(folder ${PROJECT_NAME})
set_property(GLOBAL PROPERTY FOLDER Shared)
##################################
# No Dependency
add_subdirectory(MonitorLoggerLIB/MonitorLoggerLib)
xpSourceListAppend()
