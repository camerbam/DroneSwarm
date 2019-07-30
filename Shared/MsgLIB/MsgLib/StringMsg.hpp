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

#endif