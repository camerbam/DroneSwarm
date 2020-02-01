#ifndef START_GAME_RSP_HPP
#define START_GAME_RSP_HPP

#include <string>

#include "MsgTypes.hpp"

namespace msg
{
  class StartGameRsp
  {
  public:
    StartGameRsp();
    static std::string name() { return "StartGameRsp"; }

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