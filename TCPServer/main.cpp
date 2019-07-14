#include <functional>
#include <iostream>

#include "TCPLib/TCPServer.hpp"

#include "MsgLib/BaseMsg.hpp"
#include "MsgLib/StringMsg.hpp"

int main()
{
  boost::asio::thread_pool pool(1);
  std::function<msg::BaseMsg(std::string)> parser([](std::string msg) {
    return msg::StringMsg(msg);
  });

  tcp::TcpServer Server(8080, pool, parser);

  std::function<void(std::shared_ptr<msg::BaseMsg>)> handler(
    [](std::shared_ptr<msg::BaseMsg> msg) {
      auto pMsg = std::dynamic_pointer_cast<msg::StringMsg>(msg);
      std::cout << pMsg->m_msg << std::endl;
    });

  Server.registerHandler("StringMsg", handler);

  pool.join();

  return 0;
}
