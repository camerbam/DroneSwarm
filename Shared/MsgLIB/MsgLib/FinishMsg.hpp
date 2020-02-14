#ifndef FINISH_MSG_HPP
#define FINISH_MSG_HPP

#include <string>

namespace msg
{
  class FinishMsg
  {
  public:
    FinishMsg();
    FinishMsg(int gameId);
    static std::string name() { return "FinishMsg"; }

    int gameId() const { return m_gameId; }

    void gameId(int gameId) { m_gameId = gameId; }

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