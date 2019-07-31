#include <functional>
#include <iostream>

#include "TCPLib/TCPServer.hpp"

#include "MsgLib/StringMsg.hpp"

int main()
{
  //boost::asio::thread_pool pool(1);

  //std::vector<boost::signals2::scoped_connection> connections;

  //tcp::TcpServer server(8080, pool);

  //connections.push_back(server.registerHandler<msg::StringMsg>(
  //  [](msg::StringMsg msg) { std::cout << msg.msg() << std::endl; }));

  //std::string line;
  //while (line != "quit")
  //{
  //  std::getline(std::cin, line);
  //  msg::StringMsg msg(msg::FORMAT::PROTOBUF, line);
  //  server.sendToAll(msg);
  //}

  return 0;
}
