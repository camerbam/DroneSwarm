#ifndef TCP_TOOLS_HPP
#define TCP_TOOLS_HPP

#include <string>

#include <boost/optional.hpp>

namespace tcp
{
  enum class FORMAT
  {
    STRING,
    JSON,
    PROTOBUF,
    XML
  };

  const char SYNC_WORD(1);
  const size_t SIZE_OF_SIZE(5);
  const size_t SIZE_OF_HEADER(SIZE_OF_SIZE + sizeof(char));

  std::string getProcessedString(std::string toSend);

  boost::optional<std::string> getNextStringMessage(std::string& buffer);

  // Will strip the format of of the from of the message
  // What if receives too small of string
  FORMAT getMsgFormat(std::string& msg);

} // namespace tcp

#endif
