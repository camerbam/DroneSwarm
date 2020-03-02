#include <boost/test/unit_test.hpp>

#include "TCPLib/TCPClient.hpp"
#include "TCPLib/TCPConnection.hpp"
#include "TCPLib/TCPServer.hpp"

#include "MsgLib/StringMsg.hpp"
#include "RegistryLib/Registry.hpp"

BOOST_AUTO_TEST_CASE(TCPServerSend)
{
  GlobalRegistry::setRegistry(100, 20);

  tcp::TcpServer server(8080);

  auto connection = server.registerConnection(
    [&server](std::shared_ptr<tcp::TcpConnection> pConnection) {
      pConnection->ready();

      std::vector<std::string> msgsToSend{
        "json test", "protobuf test", "xml test"};

      msg::StringMsg msg1(msgsToSend[0]);
      pConnection->send(msg1);
      msg::StringMsg msg2(msgsToSend[1]);
      pConnection->send(msg2);
      msg::StringMsg msg3(msgsToSend[2]);
      pConnection->send(msg3);

    });

  std::vector<boost::signals2::scoped_connection> connections;

  tcp::TcpClient client("localhost", "8080");

  connections.push_back(client.registerHandler<msg::StringMsg>(
    [&client](msg::StringMsg msg, const std::string&) {
    static std::vector<std::string> msgsToGet{
      "json test", "protobuf test", "xml test" };
    static size_t msgsLeft = 3;

    msgsLeft--;
    std::vector<std::string>::iterator found =
      std::find(msgsToGet.begin(), msgsToGet.end(), msg.msg());
    BOOST_CHECK(found != msgsToGet.end());
    msgsToGet.erase(std::remove(msgsToGet.begin(), msgsToGet.end(), *found),
      msgsToGet.end());
    BOOST_CHECK(msgsLeft == msgsToGet.size());
    if (msgsLeft == 0) client.close();
  }));
  client.ready();

  server.close();
}

BOOST_AUTO_TEST_CASE(TCPClientSend)
{
  std::vector<boost::signals2::scoped_connection> connections;

  tcp::TcpServer server(8080);

  auto connection = server.registerConnection([&connections, &server](
    std::shared_ptr<tcp::TcpConnection> pConnection) {
    connections.push_back(pConnection->registerHandler<msg::StringMsg>(
      [pConnection](msg::StringMsg msg, const std::string&) {
        static size_t msgsLeft = 3;
        static std::vector<std::string> msgsToGet{
          "json test", "protobuf test", "xml test"};
        msgsLeft--;
        std::vector<std::string>::iterator found =
          std::find(msgsToGet.begin(), msgsToGet.end(), msg.msg());
        BOOST_CHECK(found != msgsToGet.end());
        msgsToGet.erase(std::remove(msgsToGet.begin(), msgsToGet.end(), *found),
                        msgsToGet.end());
        BOOST_CHECK(msgsLeft == msgsToGet.size());
        if (msgsLeft == 0) pConnection->close();
      }));
    pConnection->ready();
  });

  tcp::TcpClient client("localhost", "8080");
  client.ready();

  std::vector<std::string> msgsToSend{
    "json test", "protobuf test", "xml test" };

  msg::StringMsg msg1(msgsToSend[0]);
  client.send(msg1);
  msg::StringMsg msg2(msgsToSend[1]);
  client.send(msg2);
  msg::StringMsg msg3(msgsToSend[2]);
  client.send(msg3);

  client.close();

  server.close();
}
