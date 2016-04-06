CC = /usr/bin/gcc
CFLAGS = -msse2avx -march=corei7-avx -O3 -fomit-frame-pointer -Wall -Wextra -g -mfma
PYTHON3 = python3

#-msse2avx -march=corei7-avx 

SOURCE = crypto_hash_sha512.c sign.c fastrandombytes.c cpucycles.c crypto_stream_salsa20_amd64_xmm6.s consts.c sample.c oracle.c ntt.c ntt_transform.c 
SOURCE += gauss/samplers/rejection_ber_independent/rej_ber_independent.c gauss/rnd/random.c

HEADER = api.h crypto_hash_sha512.h crypto_sign.h randombytes.h crypto_int16.h cpucycles.h fastrandombytes.h params.h gauss/samplers/rejection_ber_independent/rej_ber_independent_table.data sample.h oracle.h

#Get the sigma from the param.h file to generate the sampler table
SIGMA ?= $$(/usr/bin/gcc -dM -E params.h | grep "PARAM_SIGMA " |awk '{ print $$3}')
TAIL ?= 13

PRECISION ?= 80

all: test/test_sign \
	   test/speed 


test/test_sign: $(SOURCE) $(HEADER)  test/test_sign.c randombytes.c
	$(PYTHON3) gauss/samplers/rejection_ber_independent/gen_ber_table.py $(SIGMA) $(TAIL) $(PRECISION) > gauss/samplers/rejection_ber_independent/rej_ber_independent_table.data
	$(CC) $(CFLAGS) $(SOURCE) test/test_sign.c  -lm -o $@  randombytes.c

test/speed: $(SOURCE) $(HEADER) test/speed.c randombytes.c
	$(CC) $(CFLAGS) $(SOURCE) test/speed.c  -lm -o $@  randombytes.c

.PHONY: clean

clean:
	-rm test/test_sign
	-rm test/speed
