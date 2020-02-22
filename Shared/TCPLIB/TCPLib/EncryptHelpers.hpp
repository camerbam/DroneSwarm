#ifndef ENCRYPTED_HELPERS_HPP
#define ENCRYPTED_HELPERS_HPP

#include <iostream>
#include <string>

#include <openssl/rsa.h>
#include <boost/signals2/signal.hpp>

#include "MsgLib/MsgTypes.hpp"

namespace tcp
{
  RSA* createRSA(unsigned char* key, bool pub);
} // namespace tcp

#endif
