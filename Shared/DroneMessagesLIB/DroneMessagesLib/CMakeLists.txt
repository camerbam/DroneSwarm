set(lib_name DroneMessagesLib)
set(message_srcs
  Messages/BackMessage.cpp
  Messages/BackMessage.hpp
  Messages/BatteryMessage.cpp
  Messages/BatteryMessage.hpp
  Messages/ClockwiseMessage.cpp
  Messages/ClockwiseMessage.hpp
  Messages/CommandMessage.cpp
  Messages/CommandMessage.hpp
  Messages/CounterClockwiseMessage.cpp
  Messages/CounterClockwiseMessage.hpp
  Messages/DownMessage.cpp
  Messages/DownMessage.hpp
  Messages/FlipMessage.cpp
  Messages/FlipMessage.hpp
  Messages/ForwardMessage.cpp
  Messages/ForwardMessage.hpp
  Messages/GoMessage.cpp
  Messages/GoMessage.hpp
  Messages/LandMessage.cpp
  Messages/LandMessage.hpp
  Messages/LeftMessage.cpp
  Messages/LeftMessage.hpp
  Messages/RightMessage.cpp
  Messages/RightMessage.hpp
  Messages/SpeedMessage.cpp
  Messages/SpeedMessage.hpp
  Messages/TakeoffMessage.cpp
  Messages/TakeoffMessage.hpp
  Messages/TimeMessage.cpp
  Messages/TimeMessage.hpp
  Messages/UpMessage.cpp
  Messages/UpMessage.hpp
  )
source_group(Messages FILES ${message_srcs})
list(APPEND ${lib_name}_libsrcs ${message_srcs})
#########################################
set(base_srcs
  DroneStatusMessage.cpp
  DroneStatusMessage.hpp
  Message.cpp
  Message.hpp
  Message_t.hpp
  MessageFactory.cpp
  MessageFactory.hpp
  NoParamMessage.cpp
  NoParamMessage.hpp
  OneParamMessage.cpp
  OneParamMessage.hpp
  )
source_group("" FILES ${base_srcs})
list(APPEND ${lib_name}_libsrcs ${base_srcs})
#########################################
add_library(${lib_name} ${${lib_name}_libsrcs})
target_link_libraries(${lib_name} PUBLIC json xml)
target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
set_property(TARGET ${lib_name} PROPERTY FOLDER ${folder})
add_subdirectory(Test)
xpSourceListAppend("${${lib_name}_libsrcs}")
