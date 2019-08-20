#include "UDPLib/UDPServer.hpp"

#include <boost/asio/io_context.hpp>

#include <iostream>

int main()
{
  try
  {
    boost::asio::thread_pool pool(1);
    udp::UDPServer server(8080, pool, false);

    server.registerReceiver(
      [](std::string msg) { std::cout << msg << std::endl; });

    server.ready();

    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line == "quit") break;
      server.send(line);
    }
    server.stop();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}