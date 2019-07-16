#include <functional>
#include <iostream>

#include "TCPLib/TCPClient.hpp"

#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/StringMsg.hpp"

int main()
{
  boost::asio::thread_pool pool(1);
  std::function<msg::BaseMsg(std::string)> parser([](std::string msg) {
    return msg::StringMsg(msg);
  });

  tcp::TcpClient client("localhost", "8080", pool, parser);

  std::function<void(std::shared_ptr<msg::BaseMsg>)> handler(
    [](std::shared_ptr<msg::BaseMsg> msg) {
      auto pMsg = std::static_pointer_cast<msg::StringMsg>(msg);
      std::cout << pMsg->m_msg << std::endl;
    });

  client.registerHandler("StringMsg", handler);

  //pool.join();

  return 0;
}
