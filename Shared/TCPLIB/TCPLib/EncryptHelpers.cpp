#include "EncryptHelpers.hpp"

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

RSA* tcp::createPrivateRSA(const std::string& key)
{
  RSA* p_rsa = NULL;
  BIO* pkeybio = NULL;
  EVP_PKEY* evp_pkey = NULL;

  pkeybio = BIO_new_mem_buf((void*)key.c_str(), static_cast<int>(key.size()));
  if (pkeybio == NULL)
  {
    return nullptr;
  }
  p_rsa = PEM_read_bio_RSAPrivateKey(pkeybio, &p_rsa, NULL, NULL);
  if (p_rsa == NULL)
  {
    char buffer[120];
    ERR_error_string(ERR_get_error(), buffer);
  }
  evp_pkey = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(evp_pkey, p_rsa);

  BIO_free(pkeybio);

  return p_rsa;
}

RSA* tcp::createPublicRSA(const std::string& key)
{
  RSA* pb_rsa = NULL;
  EVP_PKEY* evp_pbkey = NULL;
  BIO* pbkeybio = NULL;
  pbkeybio = BIO_new_mem_buf((void*)key.c_str(), static_cast<int>(key.size()));
  if (pbkeybio == NULL)
  {
    return nullptr;
  }
  pb_rsa = PEM_read_bio_RSAPublicKey(pbkeybio, &pb_rsa, NULL, NULL);
  if (pb_rsa == NULL)
  {
    char buffer[120];
    ERR_error_string(ERR_get_error(), buffer);
    printf("Error reading public key:%s\n", buffer);
  }
  evp_pbkey = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(evp_pbkey, pb_rsa);

  BIO_free(pbkeybio);

  return pb_rsa;
}
