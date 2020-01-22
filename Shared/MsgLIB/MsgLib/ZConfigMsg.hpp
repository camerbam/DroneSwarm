#ifndef ZCONFIG_MSG_HPP
#define ZCONFIG_MSG_HPP

#include <string>

namespace msg
{
  class ZConfigMsg
  {
  public:
    ZConfigMsg();
    ZConfigMsg(const double& msg);
    static std::string name() { return "ZConfigMsg"; }

    double zAxis() const { return m_zAxis; }
    void zAxis(const double& zAxis) { m_zAxis = zAxis; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
    double m_zAxis;
  };
} // namespace msg

#endif