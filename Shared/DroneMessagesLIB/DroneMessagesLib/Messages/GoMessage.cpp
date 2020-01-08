#include "GoMessage.hpp"

#include <exception>

messages::GoMessage::GoMessage()
  : Message("go"),
    m_xDistance(20),
    m_yDistance(20),
    m_zDistance(20),
    m_speed(10)
{
}

messages::GoMessage::GoMessage(double x, double y, double z, size_t speed)
  : Message("go"),
    m_xDistance(x),
    m_yDistance(y),
    m_zDistance(z),
    m_speed(speed)
{
  validateDistance(m_xDistance, 20, 500);
  validateDistance(m_yDistance, 20, 500);
  validateDistance(m_zDistance, 20, 500);
  validateDistance(m_speed, 10, 100);
}

bool messages::GoMessage::fromStringImpl(std::string& toParse)
{
  try
  {
    m_xDistance = std::stod(getNextWord(toParse));
    m_yDistance = std::stod(getNextWord(toParse));
    m_zDistance = std::stod(getNextWord(toParse));
    m_speed = std::stoi(getNextWord(toParse));
  }
  catch (std::invalid_argument& ec)
  {
    throw std::runtime_error(std::string("Could not parse back message: ") +
                             ec.what());
  }
  validateDistance(m_xDistance, 20, 500);
  validateDistance(m_yDistance, 20, 500);
  validateDistance(m_zDistance, 20, 500);
  validateDistance(m_speed, 10, 100);
  return true;
}

std::string messages::GoMessage::toString() const
{
  return m_name + " " + removeUnimportantZeros(std::to_string(m_xDistance)) +
         " " + removeUnimportantZeros(std::to_string(m_yDistance)) + " " +
         removeUnimportantZeros(std::to_string(m_zDistance)) + " " +
         std::to_string(m_speed);
}

double messages::GoMessage::getXDistance() const
{
  return m_xDistance;
}

double messages::GoMessage::getYDistance() const
{
  return m_yDistance;
}

double messages::GoMessage::getZDistance() const
{
  return m_zDistance;
}

size_t messages::GoMessage::getSpeed() const
{
  return m_speed;
}

void messages::GoMessage::validateDistance(double distance,
                                           double min,
                                           double max)
{
  if (distance < min)
    throw std::runtime_error("Go message requires a number greater than " +
                             removeUnimportantZeros(std::to_string(min)));
  if (distance > max)
    throw std::runtime_error("Go message requires a number smaller than " +
                             removeUnimportantZeros(std::to_string(max)));
}

void messages::GoMessage::validateDistance(size_t distance,
                                           size_t min,
                                           size_t max)
{
  if (distance < min)
    throw std::runtime_error("Go message requires a number greater than " +
                             std::to_string(min));
  if (distance > max)
    throw std::runtime_error("Go message requires a number smaller than " +
                             std::to_string(max));
}
