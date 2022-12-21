CC=gcc
Flags= -g -pthread

all: com 

run:
	./com

com: latency.o 
	$(CC) $(Flags) -o com latency.o 

latency.o: latency.c
	$(CC) $(Flags) -c latency.c 

.PHONY: clean all

clean:
	rm -f *.o *.a com