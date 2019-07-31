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
    BaseMsg(const FORMAT& format,
            const std::string& type,
            const std::string& msg);
    static std::string name() { return "BaseMsg"; }
    bool parseString(std::string msg);

    std::string toString();

    FORMAT format() { return m_format; }
    std::string type() { return m_type; }
    std::string msg() { return m_msg; }

    void format(FORMAT format) { m_format = format; }
    void type(std::string type) { m_type = type; }
    void msg(std::string msg) { m_msg = msg; }

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