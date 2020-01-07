#ifndef UDP_RESPONSE_HPP
#define UDP_RESPONSE_HPP

#include <string>

#include <boost/asio/ip/udp.hpp>

namespace udp
{
  /**
  * Response from another process. Contains a message, and whether that is an
  * error message or not
  */
  class Response
  {
  public:
    Response();

    /** Construct a success message
    * @param[in] remoteEndpoint - Where the message came from
    * @param[in] message - Message received or error message
    */
    Response(const boost::asio::ip::udp::endpoint& remoteEndpoint,
             const std::string& message,
             const bool didSucceed);

    boost::asio::ip::udp::endpoint getEndpoint() const;
    void setEndpoint(const boost::asio::ip::udp::endpoint& endpoint);

    bool didSucceed() const;
    void setDidSucceed(const bool didSucceed);

    std::string getMessage() const;
    void setMessage(const std::string& message);

  private:
    boost::asio::ip::udp::endpoint m_remoteEndpoint;
    std::string m_message;
    bool m_didSucceed;
  };
}

#endif
