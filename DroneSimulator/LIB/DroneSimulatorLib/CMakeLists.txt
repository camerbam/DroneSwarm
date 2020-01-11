set(lib_name DroneSimulatorLib)
###############################################
set(DroneSimulator_srcs
  DroneSimulator.cpp
  DroneSimulator.hpp
  DroneSimulatorConsts.hpp
  )
source_group("" FILES ${DroneSimulator_srcs})
list(APPEND ${lib_name}_libsrcs ${DroneSimulator_srcs})
###############################################
set(DroneState_srcs
  State/DroneConfiguration.cpp
  State/DroneConfiguration.hpp
  State/DroneLocation.cpp
  State/DroneLocation.hpp
  State/DroneSimulatorIdleState.cpp
  State/DroneSimulatorIdleState.hpp
  State/DroneSimulatorSDKState.cpp
  State/DroneSimulatorSDKState.hpp
  State/DroneSimulatorState.hpp
  State/DroneSimulatorStateImpl.cpp
  State/DroneSimulatorStateImpl.hpp
  State/DroneSimulatorStateChanges.cpp
  State/DroneSimulatorStateChanges.hpp
  )
source_group(DroneState FILES ${DroneState_srcs})
list(APPEND ${lib_name}_libsrcs ${DroneState_srcs})
###############################################
set(DroneStateUpdater_srcs
  Updaters/AngleUpdater.cpp
  Updaters/AngleUpdater.hpp
  Updaters/DroneUpdater.hpp
  Updaters/LocationUpdater.cpp
  Updaters/LocationUpdater.hpp
  )
source_group(Updaters FILES ${DroneStateUpdater_srcs})
list(APPEND ${lib_name}_libsrcs ${DroneStateUpdater_srcs})
###############################################
add_library(${lib_name} ${${lib_name}_libsrcs})
target_link_libraries(${lib_name} PUBLIC DroneMessagesLib udp registry)
target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
xpSourceListAppend("${${lib_name}_libsrcs}")
