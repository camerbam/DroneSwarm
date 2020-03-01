#include "MsgTypes.hpp"

#include <map>

namespace
{
  std::map<msg::FORMAT, char> formatToChar{{msg::FORMAT::JSON, 'J'},
                                           {msg::FORMAT::PROTOBUF, 'P'},
                                           {msg::FORMAT::XML, 'X'}};

  std::map<char, msg::FORMAT> charToFormat{{'J', msg::FORMAT::JSON},
                                           {'P', msg::FORMAT::PROTOBUF},
                                           {'X', msg::FORMAT::XML}};

  std::map<std::string, msg::FORMAT> stringToChar{
    {"json", msg::FORMAT::JSON},
    {"protobuf", msg::FORMAT::PROTOBUF},
    {"xml", msg::FORMAT::XML}};

} // namespace

char msg::formatToChar(const FORMAT format)
{
  return ::formatToChar[format];
}

msg::FORMAT msg::stringToFormat(const std::string& format)
{
  return ::stringToChar[format];
}
