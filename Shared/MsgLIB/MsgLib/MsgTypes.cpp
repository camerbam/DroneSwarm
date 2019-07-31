#include "MsgTypes.hpp"

msg::FORMAT msg::getMsgFormat(std::string& msg)
{
  auto toReturn = static_cast<msg::FORMAT>((int)msg[0]);
  msg.erase(0, 1);
  return toReturn;
}
