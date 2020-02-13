#ifndef HIT_TARGET_RSP_HPP
#define HIT_TARGET_RSP_HPP

#include <string>
#include <vector>

#include "Target.hpp"

namespace msg
{
  class HitTargetRsp
  {
  public:
    HitTargetRsp();
    HitTargetRsp(bool success,
                 bool complete,
                 const std::string& error,
                 const std::vector<Target>& newTargets,
                 const std::vector<Target>& badTargets);
    static std::string name() { return "HitTargetRsp"; }

    void success(const bool& success) { m_success = success; }
    void complete(const bool& complete) { m_complete = complete; }
    void errorMsg(const std::string& errorMsg) { m_errorMsg = errorMsg; }

    void newTargets(const std::vector<Target>& newTargets) { m_newTargets = newTargets; }
    void badTargets(const std::vector<Target>& badTargets) { m_badTargets = badTargets; }
    
    bool success() { return m_success; }
    bool complete() { return m_complete; }
    std::string errorMsg() { return m_errorMsg; }
    std::vector<Target> newTargets() { return m_newTargets; }
    std::vector<Target> badTargets() { return m_badTargets; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    bool m_success;
    bool m_complete;
    std::string m_errorMsg;
    std::vector<Target> m_newTargets;
    std::vector<Target> m_badTargets;
  };
}

#endif
