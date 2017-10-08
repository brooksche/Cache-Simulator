############################################################


CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-I.
CFLAGS+= -g
CFLAGS+= -Wall
CFLAGS+= -O2

LIBS=-lm

DEPS = cache.h test.h 
OBJ = cache.o
#OBJ = cache.o lru.o snoop.o util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cachebuild: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o cachebuild
