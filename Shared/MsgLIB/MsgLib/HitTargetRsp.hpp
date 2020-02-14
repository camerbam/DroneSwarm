#ifndef HIT_TARGET_RSP_HPP
#define HIT_TARGET_RSP_HPP

#include <string>
#include <vector>

#include "TargetMsg.hpp"

namespace msg
{
  class HitTargetRsp
  {
  public:
    HitTargetRsp();
    HitTargetRsp(bool success,
                 bool complete,
                 const std::vector<TargetMsg>& newTargets,
                 const std::vector<TargetMsg>& badTargets);
    static std::string name() { return "HitTargetRsp"; }

    void success(const bool& success) { m_success = success; }
    void complete(const bool& complete) { m_complete = complete; }
    void newTargets(const std::vector<TargetMsg>& newTargets)
    {
      m_newTargets = newTargets;
    }
    void badTargets(const std::vector<TargetMsg>& badTargets)
    {
      m_badTargets = badTargets;
    }

    bool success() const { return m_success; }
    bool complete() const { return m_complete; }
    std::vector<TargetMsg> newTargets() const { return m_newTargets; }
    std::vector<TargetMsg> badTargets() const { return m_badTargets; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    bool m_success;
    bool m_complete;
    std::vector<TargetMsg> m_newTargets;
    std::vector<TargetMsg> m_badTargets;
  };
}

#endif
