#include "MessageFactory.hpp"

#include "JSONLib/Deserialize.hpp"
#include "XMLLib/Deserialize.hpp"

namespace
{
  template <class T>
  messages::Message_t createAction(rapidjson::Value& jsonToParse)
  {
    T t;
    t.readJson(jsonToParse);
    return messages::Message_t(t);
  }

  template <class T>
  messages::Message_t createAction(rapidxml::xml_node<>* xmlToParse)
  {
    T t;
    t.readXml(xmlToParse);
    return messages::Message_t(t);
  }

  template <class T>
  messages::Message_t createAction(std::string stringToParse)
  {
    T t;
    t.fromString(stringToParse);
    return messages::Message_t(t);
  }
}

messages::Message_t messages::getMessage(rapidjson::Value& jsonToParse)
{
  if (!jsonToParse.IsObject())
    throw std::runtime_error("Json message must be a object");
  auto type = json::getString(jsonToParse, "type");
  std::shared_ptr<messages::Message> toReturn;
  auto obj = json::getObjectOrArray(jsonToParse, "data");
  if (!obj) throw std::runtime_error("Message needs data for type: " + type);

  if (type == "back")
    return createAction<BackMessage>(*obj);
  else if (type == "battery")
    return createAction<BatteryMessage>(*obj);
  if (type == "clockwise" || type == "cw")
    return createAction<ClockwiseMessage>(*obj);
  else if (type == "command")
    return createAction<CommandMessage>(*obj);
  if (type == "counterclockwise" || type == "ccw")
    return createAction<CounterClockwiseMessage>(*obj);
  else if (type == "down")
    return createAction<DownMessage>(*obj);
  else if (type == "flip")
    return createAction<FlipMessage>(*obj);
  else if (type == "forward")
    return createAction<ForwardMessage>(*obj);
  else if (type == "go")
    return createAction<GoMessage>(*obj);
  else if (type == "land")
    return createAction<LandMessage>(*obj);
  else if (type == "left")
    return createAction<LeftMessage>(*obj);
  else if (type == "right")
    return createAction<RightMessage>(*obj);
  else if (type == "speed")
    return createAction<SpeedMessage>(*obj);
  else if (type == "takeoff")
    return createAction<TakeoffMessage>(*obj);
  else if (type == "time")
    return createAction<TimeMessage>(*obj);
  else if (type == "up")
    return createAction<UpMessage>(*obj);
  else
    throw std::runtime_error("No message type: " + type);
}

messages::Message_t messages::getMessage(rapidxml::xml_node<>* xmlToParse)
{
  auto typeObj = xmlToParse->first_node("type");
  if (!typeObj) throw std::runtime_error("Message need type");
  std::string type = typeObj->value();
  std::shared_ptr<messages::Message> toReturn;
  auto obj = xmlToParse->first_node("data");
  if (!obj) throw std::runtime_error("Message needs data");

  if (type == "back")
    return createAction<BackMessage>(obj);
  else if (type == "battery")
    return createAction<BatteryMessage>(obj);
  if (type == "clockwise" || type == "cw")
    return createAction<ClockwiseMessage>(obj);
  else if (type == "command")
    return createAction<CommandMessage>(obj);
  if (type == "counterclockwise" || type == "ccw")
    return createAction<CounterClockwiseMessage>(obj);
  else if (type == "down")
    return createAction<DownMessage>(obj);
  else if (type == "flip")
    return createAction<FlipMessage>(obj);
  else if (type == "forward")
    return createAction<ForwardMessage>(obj);
  else if (type == "go")
    return createAction<GoMessage>(obj);
  else if (type == "land")
    return createAction<LandMessage>(obj);
  else if (type == "left")
    return createAction<LeftMessage>(obj);
  else if (type == "right")
    return createAction<RightMessage>(obj);
  else if (type == "speed")
    return createAction<SpeedMessage>(obj);
  else if (type == "takeoff")
    return createAction<TakeoffMessage>(obj);
  else if (type == "time")
    return createAction<TimeMessage>(obj);
  else if (type == "up")
    return createAction<UpMessage>(obj);
  throw std::runtime_error("No message type: " + type);
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