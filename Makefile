CC=gcc
CFLAGS=-c -Wall
OBJECTS=gpt.o gpt_codegen.o

all: gpt clean

gpt:$(OBJECTS)
		$(CC) $(OBJECTS) -o gpt
%.o: %.c
		$(CC) $(CFLAGS) $<
clean:
		rm -rf *.o