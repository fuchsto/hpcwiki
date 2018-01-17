
CC       = gcc
CFLAGS   = -fopenmp -O3 -Ofast --std=gnu11
LDFLAGS  = -lpapi -lm
OMPP_CMD = kinst-ompp-papi

all: openmp.04.psum.ompp.a

%.ompp.a: %.c
	$(OMPP_CMD) $(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean : 
	rm -f ./opari*
	rm -f ./*.a
	rm -f ./*.opari.inc
	rm -f ./*.mod.c
