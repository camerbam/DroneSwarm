#ifndef BaseMsg_hpp
#define BaseMsg_hpp

#include <string>

#include "MsgTypes.hpp"

namespace msg
{
  class BaseMsg
  {
  public:
    BaseMsg();
    BaseMsg(const FORMAT& format, const std::string& type, const std::string& msg);
    static std::string name() { return "BaseMsg"; }
    bool parseString(std::string& msg);

    std::string toString();

  private:
    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString();
    std::string toProtoString();
    std::string toXMLString();

    FORMAT m_format;
    std::string m_type;
    std::string m_msg;
  };
} // namespace msg

#endif