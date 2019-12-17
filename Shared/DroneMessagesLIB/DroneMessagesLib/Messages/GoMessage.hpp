#ifndef GO_MESSAGES_HPP
#define GO_MESSAGES_HPP

#include "DroneMessagesLib/Message.hpp"

namespace messages
{
  class GoMessage : public Message
  {
  public:
    GoMessage();
    GoMessage(double x, double y, double z, size_t speed);
    virtual std::string toString() const override;

    double getXDistance() const;
    double getYDistance() const;
    double getZDistance() const;
    size_t getSpeed() const;

  private:
    virtual bool fromStringImpl(std::string& toParse) override;

    void validateDistance(double distance, double min, double max);
    void validateDistance(size_t distance, size_t min, size_t max);

    double m_xDistance;
    double m_yDistance;
    double m_zDistance;
    size_t m_speed;
  };
} // namespace messages

#endif