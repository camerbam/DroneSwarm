#include <functional>
#include <iostream>

#include "TCPLib/TCPClient.hpp"

#include "MsgLib/StringMsg.hpp"

int main()
{
  boost::asio::thread_pool pool(1);
  std::vector<boost::signals2::scoped_connection> connections;

  tcp::TcpClient client("localhost", "8080", pool);

  connections.push_back(client.registerHandler<msg::StringMsg>(
    [](msg::StringMsg msg) { std::cout << msg.m_msg << std::endl; }));

  std::string line;
  while (line != "quit")
  {
    std::getline(std::cin, line);
    msg::StringMsg msg{line};
    client.send(msg, tcp::FORMAT::PROTOBUF);
  }

  return 0;
}
