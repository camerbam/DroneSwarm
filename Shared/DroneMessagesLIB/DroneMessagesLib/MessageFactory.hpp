#ifndef MESSAGE_FACTORY_HPP
#define MESSAGE_FACTORY_HPP

#include <string>

#include "Message_t.hpp"

namespace messages
{
  Message_t getMessage(std::string& stringToParse);

} // namespace messages

#endif