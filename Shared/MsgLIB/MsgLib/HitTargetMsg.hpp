#ifndef HIT_TARGET_MSG_HPP
#define HIT_TARGET_MSG_HPP

#include <string>

#include "TargetMsg.hpp"

namespace msg
{
  class HitTargetMsg
  {
  public:
    HitTargetMsg();
    HitTargetMsg(const int& gameId, const int& id, const TargetMsg& target);
    static std::string name() { return "HitTargetMsg"; }

    int gameId() const { return m_gameId; }
    void gameId(const int& gameId) { m_gameId = gameId; }

    int id() const { return m_id; }
    void id(const int& id) { m_id = id; }

    TargetMsg target() const { return m_target; }
    void target(const TargetMsg& target) { m_target = target; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    int m_gameId;
    int m_id;
    TargetMsg m_target;
  };
}

#endif
