ejecutable= ej-fork
CC=gcc
CFLAGS=-g -c -Wall
LDFLAGS=-g -Wall
OBJS=ej-fork.o
SOURCES=$(OBJS:.o=.c)
HEADERS=*.h

all: $(ejecutable)

$(ejecutable): $(OBJS)
	$(CC) $(LDFLAGS)  $(OBJS) -o $@
%.o: %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@	
	
clean:
	rm -f ./*.o
	rm -f $(ejecutable)

new: clean $(ejecutable)
