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
    virtual bool readJsonImpl(rapidjson::Value& jsonToParse) override;
    virtual bool readXmlImpl(rapidxml::xml_node<>* xmlToParse) override;
  };
} // namespace messages

#endif