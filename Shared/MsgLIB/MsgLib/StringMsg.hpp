#ifndef StringMsg_hpp
#define StringMsg_hpp

#include <string>

#include "BaseMsg.hpp"

namespace msg
{
  class StringMsg : public BaseMsg
  {
  public:
    StringMsg(const std::string& msg);

    ~StringMsg();

    std::string m_msg;
  };
} // namespace msg

#endif