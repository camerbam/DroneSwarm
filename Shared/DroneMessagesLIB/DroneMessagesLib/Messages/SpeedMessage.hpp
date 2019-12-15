#ifndef SPEED_MESSAGES_HPP
#define SPEED_MESSAGES_HPP

#include "DroneMessagesLib/NoParamMessage.hpp"

namespace messages
{
  class SpeedMessage : public NoParamMessage
  {
  public:
    SpeedMessage();

    virtual std::string toString() const override final;

  private:
    virtual bool verifyMessage(std::string& toParse,
      const std::string& messageName) override;
  };
} // namespace messages

#endif
