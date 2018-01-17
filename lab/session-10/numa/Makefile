
CC     = gcc
CFLAGS = --std=gnu11 -lrt -Wall -pedantic -fopenmp
LFLAGS = -fopenmp

.DEFAULT: all
all: numa.si.a numa.pi.a

numa.si.a : numa.c
	$(CC) -o $@ $< $(CFLAGS) -D_NTHREADS=24
numa.pi.a : numa.c
	$(CC) -o $@ $< $(CFLAGS) -D_NTHREADS=24 -D_PARALLEL_INIT

run: all
	./numa.si.a -v   1024000 -h 2>&1 | tee -a measurements.csv
	./numa.si.a -v   5120000    2>&1 | tee -a measurements.csv
	./numa.si.a -v  10240000    2>&1 | tee -a measurements.csv
	./numa.si.a -v  20480000    2>&1 | tee -a measurements.csv
	./numa.si.a -v  40960000    2>&1 | tee -a measurements.csv
	./numa.si.a -v  81920000    2>&1 | tee -a measurements.csv
	./numa.si.a -v 163840000    2>&1 | tee -a measurements.csv
	./numa.pi.a -v   1024000 -h 2>&1 | tee -a measurements.csv
	./numa.pi.a -v   5120000    2>&1 | tee -a measurements.csv
	./numa.pi.a -v  10240000    2>&1 | tee -a measurements.csv
	./numa.pi.a -v  20480000    2>&1 | tee -a measurements.csv
	./numa.pi.a -v  40960000    2>&1 | tee -a measurements.csv
	./numa.pi.a -v  81920000    2>&1 | tee -a measurements.csv
	./numa.pi.a -v 163840000    2>&1 | tee -a measurements.csv

clean:
	rm -f ./numa.si.a
	rm -f ./numa.pi.a
