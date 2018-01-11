
CC = gcc
# CFLAGS = -fopenmp -O3 -Ofast -Wall -pedantic --std=gnu11

NCORES := $(shell grep -c ^processor /proc/cpuinfo)
CFLAGS = -fopenmp -O3 -Ofast --std=gnu11 -DNTHREADS=$(NCORES)

.DEFAULT: all
all : openmp.01.hello.a \
			openmp.02.pi.atomic.a \
			openmp.02.pi.a \
			openmp.03.sieve.a \
 			openmp.04.psum.a \
 			openmp.04.psum.barrier.a \
 			openmp.05.chunk-size.a

%.a : %.c
	$(CC) -o $@ $< $(CFLAGS)

clean :
	rm -f ./*.a

