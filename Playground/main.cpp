#include <iostream>
#include <thread>
#include <fstream>

#include "UDPLib/UDPCommunicatorReliable.hpp"
#include "UDPLib/UDPCommunicator.hpp"
#include "UDPLib/Response.hpp"

int main()
{
  
  std::thread t([](){
  udp::UDPCommunicator c(8890);
  std::ofstream fout("test.txt");
    while(true)
    {
   auto resp = c.receiveMessage(); 
   fout << resp.getMessage() << std::endl;
    }
  });
  

  udp::UDPCommunicatorReliable c;
  boost::asio::ip::udp::endpoint e(boost::asio::ip::address::from_string("192.168.10.1"), 8889);
  while(true)
  {
    std::string line;
    std::getline(std::cin, line);
    auto resp = c.sendMessage(line, e, boost::posix_time::seconds(20));
    std::cout << resp.getMessage() << std::endl;
  }

  return 0;
}
