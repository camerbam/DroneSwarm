#ifndef ENCRYPTED_HELPERS_HPP
#define ENCRYPTED_HELPERS_HPP

#include <iostream>
#include <string>

#include <openssl/rsa.h>
#include <boost/signals2/signal.hpp>

#include "MsgLib/MsgTypes.hpp"

namespace tcp
{
  RSA* createPrivateRSA(const std::string& key);
  RSA* createPublicRSA(const std::string& key);
} // namespace tcp

#endif
