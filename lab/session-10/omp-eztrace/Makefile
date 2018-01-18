
CC = gcc
TCC = eztrace_cc gcc

NCORES = $(shell grep -c ^processor /proc/cpuinfo)
BLOCKSIZE ?= 600000000

CFLAGS = -fopenmp -lm -lrt \
				 -std=gnu11 \
				 -Wall -Wno-unused -Wno-format \
				 -DMAX_THREADS=$(NCORES) -DBLOCK_SIZE=$(BLOCKSIZE)

LDFLAGS = -Wl,-rpath -Wl,~/opt/eztrace/1.1-7.1179/lib

.DEFAULT: all
all : openmp.03.map-range.bin \
			openmp.03.map-range.tbin \
			openmp.04.psum.bin \
			openmp.04.psum.tbin \
	    openmp.04.psum.dctree.bin \
	    openmp.04.psum.dctree.tbin \
	    openmp.04.psum.bin.arith.bin \
	    openmp.04.psum.bin.arith.tbin \
	    openmp.04.psum.barrier.bin \
	    openmp.04.psum.barrier.tbin \
	    openmp.04.psum.static.bin \
	    openmp.04.psum.static.tbin

%.bin : %.c
	- $(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

%.tbin : %.c
	- $(TCC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean :
	rm -f ./*.bin
	rm -f ./*.tbin

traceclean: clean
	rm -f ./*.opari.inc


