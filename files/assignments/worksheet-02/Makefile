# Run `make` to build the benchmark application,
# then `./benchmark.a` to run it.

CC=gcc
# Using --std=gnu11 and -lrt for clock_gettime
CFLAGS=--std=gnu11 -mno-sse -mno-mmx -mno-avx -mno-avx2 -lrt -Wall -pedantic

.DEFAULT: all
all: benchmark.a

%.a: %.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	-rm -f *.a
