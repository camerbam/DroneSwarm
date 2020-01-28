#ifndef TARGET_RSP_HPP
#define TARGET_RSP_HPP

#include <string>

namespace msg
{
  class TargetRsp
  {
  public:
    TargetRsp(const std::string& errorMsg = "");
    static std::string name() { return "TargetRsp"; }

    void errorMsg(const std::string& errorMsg) { m_errorMsg = errorMsg; }
    std::string errorMsg() { return m_errorMsg; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    std::string m_errorMsg;
  };
}

#endif
