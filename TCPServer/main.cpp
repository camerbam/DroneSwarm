#include <functional>
#include <iostream>

#include "TCPLib/TCPServer.hpp"

#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/StringMsg.hpp"

int main()
{
  boost::asio::thread_pool pool(1);
  std::function<msg::BaseMsg(std::string)> parser(
    [](std::string msg) { return msg::StringMsg(msg); });

  std::vector<boost::signals2::scoped_connection> connections;

  tcp::TcpServer server(8080, pool, parser);

  connections.push_back(server.registerHandler<msg::StringMsg>(
    [](msg::StringMsg msg) { std::cout << msg.m_msg << std::endl; }));

  std::string line;
  while (line != "quit")
  {
    std::getline(std::cin, line);
    server.sendToAll(line);
  }

  pool.join();

  return 0;
}
