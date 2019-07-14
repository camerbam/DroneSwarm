#ifndef BaseMsg_hpp
#define BaseMsg_hpp

#include <string>

namespace msg
{
  class BaseMsg
  {
  public:
    BaseMsg(const std::string& name);

    ~BaseMsg();

    virtual std::string getName();

  private:
    std::string m_name;
  };
} // namespace msg

#endif