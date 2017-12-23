
CFLAGS = -std=c++11 \
				 -fopenmp \
				 -ffast-math \
				 -finline-functions \
				 -finline-limit=10240 \
				 -Ofast

LFLAGS = -lm -fopenmp

OBJ    = pgm.o \
				 convolute.o \
				 main.o \
				 blend.o \
				 histogram.o \
				 single_cell_apply.o

CC     = gcc
CXX    = g++


%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

omp-filter.bin: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

all : filter.bin filter-mpi.bin

clean :
	rm -f *~
	rm -f a.out
	rm -f *.o
	rm -f *.bin

