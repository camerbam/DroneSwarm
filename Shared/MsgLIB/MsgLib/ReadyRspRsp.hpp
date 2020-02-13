#ifndef READY_RSP_RSP_HPP
#define READY_RSP_RSP_HPP

#include <string>

#include "MsgTypes.hpp"

namespace msg
{
  class ReadyRspRsp
  {
  public:
    ReadyRspRsp();
    ReadyRspRsp(int gameId);
    static std::string name() { return "ReadyRspRsp"; }

    void gameId(const int& gameId) { m_gameId = gameId; }
    int gameId() const { return m_gameId; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    int m_gameId;
  };
} // namespace msg

#endif
