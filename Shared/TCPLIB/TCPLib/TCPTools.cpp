#include "TcpTools.hpp"

#include <boost/asio.hpp>

#include <iostream>

namespace
{
  // TODO
  // const size_t MAX_SIZE(99999);
}

boost::optional<std::string> tcp::getNextStringMessage(
  std::string& buffer,
  const std::string& input)
{
  buffer.append(input);
  if (buffer.size() < tcp::SIZE_OF_HEADER) return boost::none;
  size_t first = buffer.find_first_of(tcp::SYNC_WORD);
  size_t size;
  try
  {
    size = std::stoi(buffer.substr(first, tcp::SIZE_OF_SIZE));
  }
  catch (...)
  {
    std::cout << "Unable to get message size, dropping packets" << std::endl;
    buffer.erase(0, tcp::SIZE_OF_HEADER);
    return boost::none;
  }
  if (buffer.size() < tcp::SIZE_OF_HEADER + size) return boost::none;
  return buffer.substr(tcp::SIZE_OF_HEADER + 1, size);
}
