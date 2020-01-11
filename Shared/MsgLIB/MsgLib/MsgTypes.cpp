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

} // namespace

char msg::formatToChar(const FORMAT format)
{
  return ::formatToChar[format];
}

