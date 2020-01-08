#ifndef ONE_PARAM_MESSAGE_HPP
#define ONE_PARAM_MESSAGE_HPP

#include "DroneMessagesLib/Message.hpp"

namespace messages
{
  class OneParamMessage : public Message
  {
  public:
    OneParamMessage(const std::string& name,
                    const std::string& key,
                    double min,
                    double max);
    virtual std::string toString() const override;

    double getArgument() const;

  protected:
    virtual bool fromStringImpl(std::string& toParse) override;

    bool validateDistance(double argument);

    double m_firstArgument;
    std::string m_key;
    double m_min;
    double m_max;
  };
} // namespace messages

#endif