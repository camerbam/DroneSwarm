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

    std::string msg() const { return m_msg; }
    void msg(const std::string& msg) { m_msg = msg; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
    std::string m_msg;
  };
} // namespace msg

#endif