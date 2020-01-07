#include <boost/test/unit_test.hpp>

#include "UDPLib/Response.hpp"

BOOST_AUTO_TEST_CASE(ResponseTest)
{
  {
    boost::asio::ip::udp::endpoint remote;
    std::string message("ok");
    bool didSucceed(false);
    udp::Response response(remote, message, didSucceed);
    BOOST_CHECK(response.didSucceed() == didSucceed);
    BOOST_CHECK(response.getMessage() == message);
    BOOST_CHECK(response.getEndpoint() == remote);
  }
  {
    boost::asio::ip::udp::endpoint remote;
    std::string message("error");
    bool didSucceed(true);
    udp::Response response(remote, message, didSucceed);
    BOOST_CHECK(response.didSucceed() == didSucceed);
    BOOST_CHECK(response.getMessage() == message);
    BOOST_CHECK(response.getEndpoint() == remote);
  }
  {
    boost::asio::ip::udp::endpoint remote;
    std::string message("100");
    bool didSucceed(false);
    udp::Response response(remote, message, didSucceed);
    BOOST_CHECK(response.didSucceed() == didSucceed);
    BOOST_CHECK(response.getMessage() == message);
    BOOST_CHECK(response.getEndpoint() == remote);
  }
}
