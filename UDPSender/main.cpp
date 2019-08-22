#include <iostream>
#include <string>

#include "UDPLib/UDPSender.hpp"
#include <boost/asio/thread_pool.hpp>

int main()
{
  boost::asio::thread_pool pool(1);
  boost::asio::ip::udp::endpoint remoteProcess(
    boost::asio::ip::address::from_string("127.0.0.1"), 8080);
  unsigned short localPort(8081);

  try
  {
    udp::UDPSender sender(localPort, pool);

    auto con = sender.registerReceiver(
      [](boost::asio::ip::udp::endpoint endpoint, std::string msg) {
      std::cout << endpoint.port() << "result" << msg << std::endl;
    });

    sender.ready();

    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line == "quit") break;
      sender.send(line, remoteProcess);
    }
    sender.stop();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  catch (...)
  {
    auto a = std::current_exception();
  }

  return 0;
}
