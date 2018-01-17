
CC = eztrace_cc gcc
NCORES = $(shell grep -c ^processor /proc/cpuinfo)
BLOCKSIZE ?= 6000000

CFLAGS = -fopenmp -lm -lrt \
				 -std=gnu11 \
				 -O3 -Ofast \
				 -Wall -pedantic -Wno-unused -Wno-format \
				 -DMAX_THREADS=$(NCORES) -DBLOCK_SIZE=$(BLOCKSIZE)

LDFLAGS = -Wl,-rpath -Wl,~/opt/eztrace/1.1-7.1179/lib

.DEFAULT: all
all : openmp.04.psum.a \
	    openmp.04.psum.dctree.a \
	    openmp.04.psum.arith.a \
	    openmp.04.psum.barrier.a \
	    openmp.04.psum.static.a

%.a : %.c
	- $(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean :
	rm -f ./*.a

traceclean: clean
	rm -f ./*.opari.inc


