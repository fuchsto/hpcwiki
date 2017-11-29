
CC_MKL_INC = -I/lrz/sys/intel/compiler/composer_xe_2015.5.223/mkl/include
CC_MKL_LIB = -L/lrz/sys/intel/compiler/composer_xe_2015.5.223/mkl/lib/intel64 -lmkl_intel_thread -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread

CC      = icc

CCFLAGS = -O3 -ipo -std=gnu11 -Wall -pedantic $(CC_MKL_INC) -fopenmp
LDFLAGS = -lrt $(CC_MKL_LIB)
OBJDIR  = ./build

ifdef USE_PAPI
	CCFLAGS += $(PAPI_INC) -DUSE_PAPI
	LDFLAGS += $(PAPI_LIB)
endif

ifeq "$(ICC_AVX)" "1"
ifeq "$(CC)" "icc"
	CCFLAGS += -xCORE-AVX2
else
	CCFLAGS += -march=core-avx2
endif
endif

all: 	$(OBJDIR) \
	$(OBJDIR)/bench.mkl.1280.aligned.a \
	$(OBJDIR)/bench.mkl.2560.aligned.a \
        \
	$(OBJDIR)/bench.ref.1280.aligned.a \
	$(OBJDIR)/bench.ref.2560.aligned.a \
        \
	$(OBJDIR)/bench.org.1280.a \
	$(OBJDIR)/bench.org.2560.a \
        \
	$(OBJDIR)/bench.tiled.1280.a \
	$(OBJDIR)/bench.tiled.2560.a \
        \
	$(OBJDIR)/bench.tiled.1280.aligned.a \
	$(OBJDIR)/bench.tiled.2560.aligned.a \
        \
	$(OBJDIR)/bench.tiled.avx.nta.1280.aligned.a \
	$(OBJDIR)/bench.tiled.avx.nta.2560.aligned.a \
        \
	$(OBJDIR)/bench.tiled.avx.1280.aligned.a \
	$(OBJDIR)/bench.tiled.avx.2560.aligned.a \
        \
	$(OBJDIR)/bench.tiled.b64.avx.1280.aligned.a \
	$(OBJDIR)/bench.tiled.b64.avx.2560.aligned.a

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/bench.%.1280.a: main.c mmult.%.c
	$(CC) -DSIZE=1280 -o $@ $(CCFLAGS) $(LDFLAGS) main.c mmult.$*.c -DVARIANT="$*" 

$(OBJDIR)/bench.%.1280.aligned.a: main.c mmult.%.c
	$(CC) -DSIZE=1280 -o $@ $(CCFLAGS) $(LDFLAGS) main.c mmult.$*.c -DVARIANT="$*" -DALIGNED

$(OBJDIR)/bench.%.2560.a: main.c mmult.%.c
	$(CC) -DSIZE=2560 -o $@ $(CCFLAGS) $(LDFLAGS) main.c mmult.$*.c -DVARIANT="$*" 

$(OBJDIR)/bench.%.2560.aligned.a: main.c mmult.%.c
	$(CC) -DSIZE=2560 -o $@ $(CCFLAGS) $(LDFLAGS) main.c mmult.$*.c -DVARIANT="$*" -DALIGNED

clean: 
	rm -f $(OBJDIR)/*.a

help:
	@echo "make [ICC_AVX=1]"
