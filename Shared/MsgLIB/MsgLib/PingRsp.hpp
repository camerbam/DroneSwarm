#ifndef PING_RSP_HPP
#define PING_RSP_HPP

#include <string>

namespace msg
{
  class PingRsp
  {
  public:
    PingRsp();
    static std::string name() { return "PingRsp"; }

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
