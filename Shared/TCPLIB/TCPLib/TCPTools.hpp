#ifndef TCP_TOOLS_HPP
#define TCP_TOOLS_HPP

#include <string>

#include <boost/optional.hpp>

#include "MsgLib/BaseMsg.hpp"

namespace tcp
{
  const char SYNC_WORD(1);
  const size_t SIZE_OF_SIZE(5);
  const size_t SIZE_OF_HEADER(SIZE_OF_SIZE + sizeof(char));

  // std::string getProcessedString(std::string toSend);

  boost::optional<std::string> getNextStringMessage(std::string& buffer,
                                                    const std::string& input);
} // namespace tcp

#endif
