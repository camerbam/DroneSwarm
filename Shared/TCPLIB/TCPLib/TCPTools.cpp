#include "TcpTools.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>

#include <iomanip>
#include <iostream>

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

boost::optional<std::string> tcp::getNextStringMessage(std::string& buffer,
                                                       std::string& input)
{
  input.erase(std::find(input.begin() + tcp::SIZE_OF_HEADER, input.end(), '\0'),
              input.end());
  buffer.append(input);
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
  return buffer.substr(tcp::SIZE_OF_HEADER, size);
}
