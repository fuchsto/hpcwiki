
CC = gcc -O3 

all : mmult.10.a mmult.100.a mmult.1000.a mmult.2000.a mmult.4000.a  mmult.8000.a 

mmult.10.a : mmult.c
	$(CC) -DSIZE=10 -o $@ mmult.c

mmult.100.a : mmult.c
	$(CC) -DSIZE=100 -o $@ mmult.c

mmult.1000.a : mmult.c
	$(CC) -DSIZE=1000 -o $@ mmult.c 

mmult.2000.a : mmult.c
	$(CC) -DSIZE=2000 -o $@ mmult.c 

mmult.4000.a : mmult.c
	$(CC) -DSIZE=4000 -o $@ mmult.c 

mmult.8000.a : mmult.c
	$(CC) -DSIZE=8000 -o $@ mmult.c 


clean : 
	rm -f ./mmult.*.a

