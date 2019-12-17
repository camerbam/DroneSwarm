#include "MessageFactory.hpp"

namespace
{
  template <class T>
  messages::Message_t createAction(std::string stringToParse)
  {
    T t;
    t.fromString(stringToParse);
    return messages::Message_t(t);
  }
}

messages::Message_t messages::getMessage(std::string& stringToParse)
{
  auto spot = stringToParse.find(" ");
  auto type = stringToParse.substr(0, spot);
  std::shared_ptr<messages::Message> toReturn;
  if (type == "back") return createAction<messages::BackMessage>(stringToParse);
  if (type == "battery" || type == "battery?")
    return createAction<messages::BatteryMessage>(stringToParse);
  if (type == "clockwise" || type == "cw")
    return createAction<messages::ClockwiseMessage>(stringToParse);
  if (type == "command")
    return createAction<messages::CommandMessage>(stringToParse);
  if (type == "counterclockwise" || type == "ccw")
    return createAction<messages::CounterClockwiseMessage>(stringToParse);
  if (type == "down") return createAction<messages::DownMessage>(stringToParse);
  if (type == "flip") return createAction<messages::FlipMessage>(stringToParse);
  if (type == "forward")
    return createAction<messages::ForwardMessage>(stringToParse);
  if (type == "go") return createAction<messages::GoMessage>(stringToParse);
  if (type == "land") return createAction<messages::LandMessage>(stringToParse);
  if (type == "left") return createAction<messages::LeftMessage>(stringToParse);
  if (type == "right")
    return createAction<messages::RightMessage>(stringToParse);
  if (type == "speed" || type == "speed?")
    return createAction<messages::SpeedMessage>(stringToParse);
  if (type == "takeoff")
    return createAction<messages::TakeoffMessage>(stringToParse);
  if (type == "time" || type == "time?")
    return createAction<messages::TimeMessage>(stringToParse);
  if (type == "up") return createAction<messages::UpMessage>(stringToParse);
  throw std::runtime_error("No message type: " + type);
}