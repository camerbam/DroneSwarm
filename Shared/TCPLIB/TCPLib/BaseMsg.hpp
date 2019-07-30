#ifndef BaseMsg_hpp
#define BaseMsg_hpp

#include <string>

#include "TCPTools.hpp"

namespace tcp
{
  class BaseMsg
  {
  public:
    // Necessary parts TODO use virtual functions to enforce this
    static std::string name() { return "BaseMsg"; }
    bool parseString(std::string msg);

    std::string toString();

    std::string toJsonString();
    std::string toProtobufString();
    std::string toXMLString();
    // End necessary Parts

    std::string type() { return m_type; }
    void type(const std::string& type) { m_type = type; }

    std::string msg() { return m_msg; }
    void msg(const std::string& msg) { m_msg = msg; }

    FORMAT format() { return m_format; }
    void format(const FORMAT& format) { m_format = format; }

  private:
    FORMAT m_format;
    std::string m_type;
    std::string m_msg;
  };
} // namespace tcp

#endif