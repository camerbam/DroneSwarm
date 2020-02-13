#ifndef PING_HPP
#define PING_HPP

#include <string>

namespace msg
{
  class PingMsg
  {
  public:
    PingMsg();
    static std::string name() { return "PingMsg"; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
  };
}

#endif
