#ifndef TIME_MESSAGES_HPP
#define TIME_MESSAGES_HPP

#include "DroneMessagesLib/NoParamMessage.hpp"

namespace messages
{
  class TimeMessage : public NoParamMessage
  {
  public:
    TimeMessage();

    virtual std::string toString() const override final;

  private:
    virtual bool verifyMessage(std::string& toParse,
                               const std::string& messageName) override;
  };
} // namespace messages

#endif