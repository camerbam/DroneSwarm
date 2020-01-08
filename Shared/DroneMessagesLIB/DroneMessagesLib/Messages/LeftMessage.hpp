#ifndef LEFT_MESSAGES_HPP
#define LEFT_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class LeftMessage : public OneParamMessage
  {
  public:
    LeftMessage();
    LeftMessage(double distance);

  private:
  };
} // namespace messages

#endif
