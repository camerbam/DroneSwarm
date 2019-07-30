#ifndef MsgTypes_HPP
#define MsgTypes_HPP

namespace msg
{
  enum class FORMAT
  {
    JSON,
    PROTOBUF,
    XML
  };

  static msg::FORMAT getMsgFormat(std::string& msg)
  {
    auto toReturn = static_cast<msg::FORMAT>((int)msg[0]);
    msg.erase(0, 1);
    return toReturn;
  }
}

#endif