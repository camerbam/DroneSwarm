#ifndef StringMsg_hpp
#define StringMsg_hpp

#include <string>

#include "MsgTypes.hpp"

namespace msg
{
  class StringMsg
  {
  public:
    StringMsg();
    StringMsg(const std::string& msg);
    static std::string name() { return "StringMsg"; }
    bool parseString(const std::string& msg, const msg::FORMAT& format);

    std::string toString(const msg::FORMAT& format);

    std::string msg() { return m_msg; }

    void msg(const std::string& msg) { m_msg = msg; }

  private:
    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString();
    std::string toProtoString();
    std::string toXMLString();

    std::string m_msg;
  };
} // namespace msg

#endif