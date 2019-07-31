#include "MsgTypes.hpp"

#include <map>

namespace
{
  std::map<msg::FORMAT, char> formatToChar{{msg::FORMAT::JSON, 'A'},
                                           {msg::FORMAT::PROTOBUF, 'B'},
                                           {msg::FORMAT::XML, 'C'}};

  std::map<char, msg::FORMAT> charToFormat{{'A', msg::FORMAT::JSON},
                                           {'B', msg::FORMAT::PROTOBUF},
                                           {'C', msg::FORMAT::XML}};

} // namespace

char msg::formatToChar(const FORMAT format)
{
  return ::formatToChar[format];
}

msg::FORMAT msg::getMsgFormat(std::string& msg)
{
  auto toReturn = ::charToFormat[msg[0]];
  msg.erase(0, 1);
  return toReturn;
}
