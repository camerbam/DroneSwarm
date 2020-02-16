#ifndef BaseMsg_hpp
#define BaseMsg_hpp

#include <chrono>
#include <string>

#include "MsgTypes.hpp"

namespace msg
{
  class BaseMsg
  {
  public:
    BaseMsg();
    BaseMsg(const std::string& msgId,
            const std::string& type,
            const std::string& msg);
    static std::string name() { return "BaseMsg"; }

    std::string msgId() const { return m_msgId; }
    std::string type() const { return m_type; }
    std::string msg() const { return m_msg; }

    void msgId(std::string msgId) { m_msgId = msgId; }
    void type(std::string type) { m_type = type; }
    void msg(std::string msg) { m_msg = msg; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;

  private:
    std::string m_msgId;
    std::string m_type;
    std::string m_msg;
  };

  struct ResendMsg
  {
    BaseMsg msg;
    std::chrono::steady_clock::time_point expireTime;
    int retries;
    ResendMsg(BaseMsg msg, std::chrono::steady_clock::time_point expireTime)
      : msg(msg), expireTime(expireTime), retries(0)
    {
    }

    ResendMsg() : msg(), expireTime(), retries(0) {}
  };
} // namespace msg

#endif