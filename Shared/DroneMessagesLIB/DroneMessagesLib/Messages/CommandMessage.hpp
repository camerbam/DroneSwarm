#ifndef COMMAND_MESSAGES_HPP
#define COMMAND_MESSAGES_HPP

#include "DroneMessagesLib/NoParamMessage.hpp"

namespace messages
{
  class CommandMessage : public NoParamMessage
  {
  public:
    CommandMessage();

  private:
  };
} // namespace messages

#endif