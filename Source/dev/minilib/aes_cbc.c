/*
 * Copyright 2002-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#include "aes.h"
//#include "modes.h"
typedef void (*block128_f) (const unsigned char in[16],
                            unsigned char out[16], const void *key);

extern void CRYPTO_cbc128_encrypt(const unsigned char *in, unsigned char *out,
        size_t len, const void *key,
        unsigned char ivec[16], block128_f block);
extern void CRYPTO_cbc128_decrypt(const unsigned char *in, unsigned char *out,
        size_t len, const void *key,
        unsigned char ivec[16], block128_f block);

void AES_cbc_encrypt(const unsigned char *in, unsigned char *out,
                     size_t len, const AES_KEY *key,
                     unsigned char *ivec, const int enc)
{

    if (enc)
        CRYPTO_cbc128_encrypt(in, out, len, key, ivec,
                              (block128_f) AES_encrypt);
    else
        CRYPTO_cbc128_decrypt(in, out, len, key, ivec,
                              (block128_f) AES_decrypt);
}
