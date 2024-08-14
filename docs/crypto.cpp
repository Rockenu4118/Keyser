#include <openssl/pem.h>

void printPKEY()
{
    EVP_PKEY* my_pkey = nullptr;

    // print to stdout
    BIO* bp = BIO_new_fp(stdout, BIO_NOCLOSE);

    EVP_PKEY_print_private(bp, my_pkey, 1, nullptr);

    // print in PEM form
    PEM_write_PrivateKey(stdout, my_pkey, NULL, NULL, 0, NULL, NULL);
}