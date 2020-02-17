#ifndef READY_MSG_hpp
#define READY_MSG_hpp

#include <string>

#include "MsgTypes.hpp"

namespace msg
{
  class ReadyMsg
  {
  public:
    ReadyMsg();
    static std::string name() { return "ReadyMsg"; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
  };
} // namespace msg

#endif