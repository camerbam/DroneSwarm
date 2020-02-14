#ifndef ZCONFIG_RSP_HPP
#define ZCONFIG_RSP_HPP

#include <string>

namespace msg
{
  class ZConfigRsp
  {
  public:
    ZConfigRsp();
    static std::string name() { return "ZConfigRsp"; }

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
