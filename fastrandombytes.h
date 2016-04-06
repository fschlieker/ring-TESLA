#ifndef FASTRANDOMBYTES_H
#define FASTRANDOMBYTES_H

void fastrandombytes(unsigned char *r, unsigned long long rlen);
void get_random_key();
void set_key(const unsigned char *data, unsigned long long datalen);
void set_zero_key(); 

#endif
