#include "StringMsg.hpp"

msg::StringMsg::StringMsg(const std::string& msg)
  :BaseMsg("StringMsg"), m_msg(msg)
{
}

msg::StringMsg::~StringMsg()
{
}
