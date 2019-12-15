#ifndef FLIP_MESSAGES_HPP
#define FLIP_MESSAGES_HPP

#include "DroneMessagesLib/Message.hpp"

namespace messages
{
  enum class Direction
  {
    FORWARD,
    RIGHT,
    BACK,
    LEFT
  };

  class FlipMessage : public Message
  {
  public:
    FlipMessage();
    FlipMessage(std::string direction);
    virtual std::string toString() const override;

    Direction getDirection() const;

  private:
    virtual bool fromStringImpl(std::string& toParse) override;
    virtual bool readJsonImpl(rapidjson::Value& jsonToParse) override;
    virtual bool readXmlImpl(rapidxml::xml_node<>* xmlToParse) override;

    Direction m_direction;
  };
} // namespace messages

#endif