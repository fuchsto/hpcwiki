# Run `make` to build the benchmark application,
# then `./benchmark.a` to run it.

CC=gcc
# Using --std=gnu11 and -lrt for clock_gettime
CFLAGS=--std=gnu11 -mno-mmx -mno-sse -lrt -Wall -pedantic

.DEFAULT: all
all: benchmark-int-add.a \
		 benchmark-int-div.a \
		 benchmark-int-mul.a \
	   benchmark-float-add.a \
		 benchmark-float-div.a \
		 benchmark-float-mul.a \
	   benchmark-double-add.a \
		 benchmark-double-div.a \
		 benchmark-double-mul.a

benchmark-int-add.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_INT -D_BENCH_OP_ADD

benchmark-int-div.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_INT -D_BENCH_OP_DIV

benchmark-int-mul.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_INT -D_BENCH_OP_MUL

benchmark-float-add.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_FLOAT -D_BENCH_OP_ADD

benchmark-float-div.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_FLOAT -D_BENCH_OP_DIV

benchmark-float-mul.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_FLOAT -D_BENCH_OP_MUL

benchmark-double-add.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_DOUBLE -D_BENCH_OP_ADD

benchmark-double-div.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_DOUBLE -D_BENCH_OP_DIV

benchmark-double-mul.a : benchmark.c
	$(CC) -o $@ $< $(CFLAGS) -D_BENCH_TYPE_DOUBLE -D_BENCH_OP_MUL

run: all
	./benchmark-int-add.a -h >> measurements.csv
	./benchmark-int-div.a    >> measurements.csv
	./benchmark-int-mul.a    >> measurements.csv
	./benchmark-float-add.a  >> measurements.csv
	./benchmark-float-div.a  >> measurements.csv
	./benchmark-float-mul.a  >> measurements.csv
	./benchmark-double-add.a >> measurements.csv
	./benchmark-double-div.a >> measurements.csv
	./benchmark-double-mul.a >> measurements.csv
	cat measurements.csv

# %.a: %.c
# 	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	-rm -f *.a
	-rm -f *.csv
