#ifndef FINISH_MSG_HPP
#define FINISH_MSG_HPP

#include <string>

namespace msg
{
  class FinishMsg
  {
  public:
    FinishMsg();
    static std::string name() { return "FinishMsg"; }

    bool parseFromJson(const std::string& msg);
    bool parseFromProto(const std::string& msg);
    bool parseFromXml(const std::string& msg);

    std::string toJsonString() const;
    std::string toProtoString() const;
    std::string toXMLString() const;
  private:
  };
} // namespace msg

#endif