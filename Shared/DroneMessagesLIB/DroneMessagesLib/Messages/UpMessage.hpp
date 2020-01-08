#ifndef UP_MESSAGES_HPP
#define UP_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class UpMessage : public OneParamMessage
  {
  public:
    UpMessage();
    UpMessage(double distance);

  private:
  };
} // namespace messages

#endif
