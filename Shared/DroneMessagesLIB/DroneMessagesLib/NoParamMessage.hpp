#ifndef NO_PARAM_MESSAGE_HPP
#define NO_PARAM_MESSAGE_HPP

#include "DroneMessagesLib/Message.hpp"

namespace messages
{
  class NoParamMessage : public Message
  {
  public:
    NoParamMessage(const std::string& name);

  protected:
    virtual bool fromStringImpl(std::string& toParse) override;
  };
} // namespace messages

#endif