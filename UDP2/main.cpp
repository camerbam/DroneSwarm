#include "UDPLib/UDPClient.hpp"

#include <boost/asio/io_context.hpp>

#include <iostream>

//int main()
//{
//  try
//  {
//    UDPClient client("127.0.0.1", "8080", true);
//    client.receive();
//    client.receive();
//  }
//  catch (std::exception& e)
//  {
//    std::cerr << e.what() << std::endl;
//  }
//  catch(...)
//  {
//    auto a = std::current_exception();
//  }
//
//  return 0;
//}

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
      *resolver.resolve(udp::v4(), "localhost", "8080").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    boost::array<char, 1> send_buf = { { 0 } };
    socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    boost::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(
      boost::asio::buffer(recv_buf), sender_endpoint);

    std::cout.write(recv_buf.data(), len);

    boost::array<char, 6> send_buf2 = { {'a', 'b', 'c', 'd', 'e', 0 } };
    socket.send_to(boost::asio::buffer(send_buf2), receiver_endpoint);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
