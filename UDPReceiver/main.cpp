#include <iostream>
#include <string>

#include "UDPLib/UDPSender.hpp"
#include <boost/asio/thread_pool.hpp>

int main()
{
  boost::asio::thread_pool pool(1);
  unsigned short localPort(8080);
  boost::signals2::scoped_connection con;

  try
  {
    udp::UDPSender receiver(localPort, pool);

     con = receiver.registerReceiver(
      [&receiver](boost::asio::ip::udp::endpoint endpoint, std::string msg) {
        std::cout << endpoint.port() << "result" << msg << std::endl;
        receiver.send(msg, endpoint);
        if(msg == "quit")
          receiver.stop();
      });

    receiver.ready();
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
