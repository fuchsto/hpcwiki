# Run `make` to build the application, then `make run` to run it.

# On SuperMUC, make sure to load the PAPI module:
#
#   $ module load papi
#

PAPI_INCLUDE_PATH=$(PAPI_HOME)/include
PAPI_LIBRARY_PATH=$(PAPI_HOME)/lib

CC      = gcc
CFLAGS  = --std=gnu11 -Wall -pedantic -I$(PAPI_INCLUDE_PATH) -Ofast
LDFLAGS = -L$(PAPI_LIBRARY_PATH) -lpapi -lrt

.DEFAULT: all
all: mmult.papi.naive.a mmult.papi.opt.a

mmult.papi.opt.a : mmult.papi.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) -DSCENARIO_OPT
mmult.papi.naive.a : mmult.papi.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) -DSCENARIO_NAIVE

run: all
	./mmult.papi.naive.a -h >> measurements.csv
	./mmult.papi.opt.a      >> measurements.csv
	cat measurements.csv

.PHONY: clean
clean:
	-rm -f *.a
	-rm -f *.csv
