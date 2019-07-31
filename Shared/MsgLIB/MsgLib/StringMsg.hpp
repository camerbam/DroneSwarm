<<<<<<< HEAD
#ifndef StringMsg_hpp
#define StringMsg_hpp

#include <string>

namespace msg
{
  class StringMsg
  {
  public:
    // Necessary parts TODO use virtual functions to enforce this
    static std::string name() { return "StringMsg"; };
    bool parseString(std::string msg);

    std::string toString();

    std::string toJsonString();
    std::string toProtobufString();
    std::string toXMLString();
    // End necessary Parts

    std::string m_msg;
  };
} // namespace msg

=======
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
    StringMsg(const FORMAT& format, const std::string& msg);
    static std::string name() { return "StringMsg"; }
    bool parseString(std::string& msg);

    std::string toString();

    FORMAT format() { return m_format; }
    std::string msg() { return m_msg; }

    void format(const FORMAT& format) { m_format = format; }
    void msg(const std::string& msg) { m_msg = msg; }

  private:
    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString();
    std::string toProtoString();
    std::string toXMLString();

    FORMAT m_format;
    std::string m_msg;
  };
} // namespace msg

>>>>>>> cameron/development
#endif