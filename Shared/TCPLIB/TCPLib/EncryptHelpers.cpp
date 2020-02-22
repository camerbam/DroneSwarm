#include "EncryptHelpers.hpp"

#include <openssl/pem.h>
#include <openssl/rsa.h>

RSA* tcp::createRSA(unsigned char* key, bool pub)
{
  RSA* rsa = NULL;
  BIO* keybio;
  keybio = BIO_new_mem_buf(key, -1);
  if (keybio == NULL)
  {
    printf("Failed to create key BIO");
    return 0;
  }
  if (pub)
  {
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
  }
  else
  {
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
  }
  if (rsa == NULL)
  {
    printf("Failed to create RSA");
  }

  return rsa;
}