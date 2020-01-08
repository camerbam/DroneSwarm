#ifndef TAKEOFF_MESSAGES_HPP
#define TAKEOFF_MESSAGES_HPP

#include "DroneMessagesLib/NoParamMessage.hpp"

namespace messages
{
  class TakeoffMessage : public NoParamMessage
  {
  public:
    TakeoffMessage();

  private:
  };
} // namespace messages

#endif
