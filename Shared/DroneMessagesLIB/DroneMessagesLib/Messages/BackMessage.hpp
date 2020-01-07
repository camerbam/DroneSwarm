#ifndef BACK_MESSAGES_HPP
#define BACK_MESSAGES_HPP

#include "DroneMessagesLib/OneParamMessage.hpp"

namespace messages
{
  class BackMessage : public OneParamMessage
  {
  public:
    BackMessage();
    BackMessage(const double& distance);

  private:
  };
} // namespace messages

#endif