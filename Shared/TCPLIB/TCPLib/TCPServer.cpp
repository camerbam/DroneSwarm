#include "TCPServer.hpp"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/write.hpp>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#include "MsgLib/BaseMsg.hpp"
#include "TCPConnection.hpp"
#include "TCPTools.hpp"

namespace
{
  // https://stackoverflow.com/questions/50363097/c-openssl-generate-rsa-keypair-and-read
  bool generateKey(std::string& pubk, std::string& prik)
  {
    int pri_len;   // Length of private key
    int pub_len;   // Length of public key
    char* pri_key; // Private key in PEM
    char* pub_key; // Public key in PEM

    int ret = 0;
    RSA* r = NULL;
    BIGNUM* bne = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;
    int bits = 2048;
    unsigned long e = RSA_F4;

    RSA* p_rsa = NULL;
    RSA* pb_rsa = NULL;
    EVP_PKEY* evp_pbkey = NULL;
    EVP_PKEY* evp_pkey = NULL;

    BIO* pbkeybio = NULL;
    BIO* pkeybio = NULL;

    // 1. generate rsa key
    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret == 1)
    {
      r = RSA_new();
      ret = RSA_generate_key_ex(r, bits, bne, NULL);
      if (ret == 1)
      {
        // 2. save public key
        // bp_public = BIO_new_file("public.pem", "w+");
        bp_public = BIO_new(BIO_s_mem());
        ret = PEM_write_bio_RSAPublicKey(bp_public, r);
        if (ret == 1)
        {

          // 3. save private key
          // bp_private = BIO_new_file("private.pem", "w+");
          bp_private = BIO_new(BIO_s_mem());
          ret = PEM_write_bio_RSAPrivateKey(
            bp_private, r, NULL, NULL, 0, NULL, NULL);

          // 4. Get the keys are PEM formatted strings
          pri_len = BIO_pending(bp_private);
          pub_len = BIO_pending(bp_public);

          pri_key = (char*)malloc(pri_len + 1);
          pub_key = (char*)malloc(pub_len + 1);

          BIO_read(bp_private, pri_key, pri_len);
          BIO_read(bp_public, pub_key, pub_len);

          pri_key[pri_len] = '\0';
          pub_key[pub_len] = '\0';

          prik = std::string(pri_key, pri_len);
          pubk = std::string(pub_key, pub_len);

          free(pri_key);
          free(pub_key);
        }
      }
    }

    // 4. free
    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(r);
    BN_free(bne);

    return (ret == 1);
  }
}

tcp::TcpServer::TcpServer(unsigned short port, msg::FORMAT format)
  : m_encrypted(GlobalRegistry::getRegistry().isEncypted()),
    m_publicKey(),
    m_privateKey(),
    m_pCtx(std::make_shared<boost::asio::io_context>()),
    m_optCork(*m_pCtx),
    m_format(format),
    m_timer(*m_pCtx, boost::asio::chrono::seconds(1)),
    m_pAcceptor(
      *m_pCtx,
      boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_connections(),
    m_connectionHandler(),
    m_iocThread([pCtx = m_pCtx]() { pCtx->run(); })
{
  if (m_encrypted) generateKey(m_publicKey, m_privateKey);
  startAccept();
}

tcp::TcpServer::~TcpServer()
{
  m_optCork = boost::none;
  if (m_iocThread.joinable()) m_iocThread.join();
}

void tcp::TcpServer::startAccept()
{
  auto pSocket = std::make_shared<boost::asio::ip::tcp::socket>(*m_pCtx);
  m_pAcceptor.async_accept(
    *pSocket, [&, pSocket](auto ec) { handleAccept(pSocket, ec); });
}

void tcp::TcpServer::handleAccept(
  std::shared_ptr<boost::asio::ip::tcp::socket> pSocket,
  const boost::system::error_code& error)
{
  if (!error)
  {
    static int m_nextId = 0;
    std::shared_ptr<TcpConnection> newConnection =
      TcpConnection::create(pSocket, m_nextId++, m_format, m_privateKey);
    newConnection->startRead();
    m_connections[newConnection->getId()] = newConnection;
    if (m_encrypted)
      pSocket->send(boost::asio::buffer(m_publicKey, m_publicKey.size()));
    newConnection->registerDisconnect([m_connections = &m_connections](int id) {
      m_connections->erase(id);
    });
    m_connectionHandler(newConnection);
  }
  startAccept();
}

void tcp::TcpServer::close()
{
  m_timer.cancel();
  m_optCork = boost::none;
  m_pCtx->stop();
}

void tcp::TcpServer::checkMsgs()
{
  m_timer.async_wait([this](const boost::system::error_code& e) {
    if (!e) checkMsgs();
  });
  auto now = std::chrono::steady_clock::now();
  for (auto m : m_connections)
    m.second->checkMsgs(now);
}

void tcp::TcpServer::setEncrypted()
{
  m_encrypted = true;
  generateKey(m_publicKey, m_privateKey);
  std::cout << "generated" << std::endl;
}
