#include "BaseMsg.hpp"

msg::BaseMsg::BaseMsg(const std::string& name) : m_name(name) {}

msg::BaseMsg::~BaseMsg() {}

std::string msg::BaseMsg::getName()
{
  return m_name;
}
