#include "TcpTools.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>

#include <iomanip>
#include <iostream>
#include <string>

namespace
{
  // TODO
  // const size_t MAX_SIZE(99999);
}

std::string tcp::getProcessedString(std::string msg)
{
  std::stringstream ss;
  ss << SYNC_WORD << std::setw(SIZE_OF_SIZE) << msg.size() << msg;
  return ss.str();
}

boost::optional<std::string> tcp::getNextStringMessage(std::string& buffer)
{
  if (buffer.size() < tcp::SIZE_OF_HEADER) return boost::none;
  size_t first = buffer.find_first_of(tcp::SYNC_WORD);
  size_t size;
  try
  {
    size = std::stoi(buffer.substr(first + 1, tcp::SIZE_OF_SIZE));
  }
  catch (...)
  {
    std::cout << "Unable to get message size, dropping packets" << std::endl;
    buffer.erase(0, tcp::SIZE_OF_HEADER);
    return boost::none;
  }
  if (buffer.size() < tcp::SIZE_OF_HEADER + size) return boost::none;
  auto toReturn = buffer.substr(tcp::SIZE_OF_HEADER, size);
  buffer.erase(buffer.begin(), buffer.begin() + tcp::SIZE_OF_HEADER + size);
  return toReturn;
}

tcp::FORMAT tcp::getMsgFormat(std::string& msg)
{
  auto toReturn = static_cast<tcp::FORMAT>((int)msg[0]);
  msg.erase(0, 1);
  return toReturn;
}
