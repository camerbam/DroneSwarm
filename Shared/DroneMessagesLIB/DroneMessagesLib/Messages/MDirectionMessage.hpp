#ifndef MDirection_MESSAGES_HPP
#define MDirection_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class MDirectionMessage : public OneParamMessage
  {
  public:
    MDirectionMessage();
    MDirectionMessage(double direction);

  private:
  };
} // namespace messages

#endif