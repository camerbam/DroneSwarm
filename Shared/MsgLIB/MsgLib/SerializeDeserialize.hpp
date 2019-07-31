#ifndef MsgTypes_HPP
#define MsgTypes_HPP

#include <string>

namespace msg
{
  template <class T>
  std::string toString(const T& msg, const msg::FORMAT& format)
  {
    switch (format)
    {
    case msg::FORMAT::JSON:
      return msg.toJsonString();
    case msg::FORMAT::PROTOBUF:
      return msg.toProtoString();
    case msg::FORMAT::XML:
      return msg.toXMLString();
    default:
      std::cout << "Unrecgnized format" << std::endl;
      return "";
    }
  }

  template <class T>
  std::string parseString(const T& msg,
                          const std::string& data,
                          const msg::FORMAT& format)
  {
    switch (format)
    {
    case msg::FORMAT::JSON:
      return msg.parseFromJson(data);
    case msg::FORMAT::PROTOBUF:
      return msg.parseFromProto(data);
    case msg::FORMAT::XML:
      return msg.parseFromXml(data);
    default:
      std::cout << "Unrecgnized format" << std::endl;
      return "";
    }
  }
} // namespace msg

#endif