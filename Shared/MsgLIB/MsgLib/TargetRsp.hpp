#ifndef TARGET_RSP_HPP
#define TARGET_RSP_HPP

#include <string>

namespace msg
{
  class TargetRsp
  {
  public:
    TargetRsp();
    static std::string name() { return "TargetRsp"; }

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
