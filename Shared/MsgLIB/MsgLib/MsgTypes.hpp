#ifndef MsgTypes_HPP
#define MsgTypes_HPP

#include <string>

namespace msg
{
  enum class FORMAT
  {
    JSON,
    PROTOBUF,
    XML
  };

  msg::FORMAT getMsgFormat(std::string& msg);
}

#endif