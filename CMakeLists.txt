cmake_minimum_required(VERSION 3.12.0)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH
  "Locaction for installation"
  FORCE)
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
  set(CMAKE_BUILD_TYPE "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
    "Choose the type to build, options are: Debug Release" FORCE)
endif()
project(DroneSwarm)
set(externprolite_REV 19.07.1)
find_package(externprolite REQUIRED)
xpSetFlags()
set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(Shared)
add_subdirectory(Drone)
add_subdirectory(DroneManager)
add_subdirectory(DroneSimulator)
add_subdirectory(GroundStation)
add_subdirectory(MockGroundStation)
add_subdirectory(Monitor)
add_subdirectory(Playground)
add_subdirectory(Referee)
###########################################
set(random_srcs
  100Times.txt
  Effectiveness.txt
  Findexternprolite.cmake
  )
xpSourceListAppend(${random_srcs})
