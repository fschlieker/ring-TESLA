#ifndef CRYPTO_STREAM_H
#define CRYPTO_STREAM_H

#define crypto_stream_KEYBYTES 32
#define crypto_stream_NONCEBYTES 8

#define crypto_stream crypto_stream_salsa20_amd64_xmm6

int crypto_stream(
        unsigned char *c,unsigned long long clen,
  const unsigned char *n,
  const unsigned char *k
);

#endif
