#ifndef BATTERY_MESSAGES_HPP
#define BATTERY_MESSAGES_HPP

#include "DroneMessagesLib/NoParamMessage.hpp"

namespace messages
{
  class BatteryMessage : public NoParamMessage
  {
  public:
    BatteryMessage();

    virtual std::string toString() const override final;

  private:

    virtual bool verifyMessage(std::string& toParse,
                               const std::string& messageName) override;
  };
} // namespace messages

#endif