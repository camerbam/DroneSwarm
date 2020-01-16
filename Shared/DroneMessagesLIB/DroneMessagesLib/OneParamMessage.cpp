#include "OneParamMessage.hpp"

namespace
{
  const size_t n_precision(2);
}

messages::OneParamMessage::OneParamMessage(const std::string& name,
                                           double min,
                                           double max)
  : Message(name), m_min(min), m_max(max)
{
}

bool messages::OneParamMessage::fromStringImpl(std::string& toParse)
{
  try
  {
    m_firstArgument = std::stod(getNextWord(toParse));
  }
  catch (std::invalid_argument& ec)
  {
    throw std::runtime_error(std::string("Could not parse back message: ") +
                             ec.what());
  }
  return validateDistance(m_firstArgument);
}

std::string messages::OneParamMessage::toString() const
{
  return m_name + " " + removeUnimportantZeros(std::to_string(m_firstArgument));
}

double messages::OneParamMessage::getArgument() const
{
  return m_firstArgument;
}

bool messages::OneParamMessage::validateDistance(double distance)
{
  if (distance < m_min)
    throw std::runtime_error(m_name +
                             " message requires a number greater than " +
                             removeUnimportantZeros(std::to_string(m_min)));
  if (distance > m_max)
    throw std::runtime_error(m_name +
                             "Back message requires a number smaller than " +
                             removeUnimportantZeros(std::to_string(m_max)));
  return true;
}
