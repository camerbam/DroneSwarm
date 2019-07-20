#include "StringMsg.hpp"

// todo Finish this

bool msg::StringMsg::parseString(std::string msg)
{
  m_msg = msg;
  return true;
}

std::string msg::StringMsg::toString()
{
  return m_msg;
}

std::string msg::StringMsg::toJsonString()
{
  return m_msg;
}
std::string msg::StringMsg::toProtobufString()
{
  return m_msg;
}
std::string msg::StringMsg::toXMLString()
{
  return m_msg;
}
