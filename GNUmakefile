CC?=gcc

CSRC:=$(wildcard *.c)
COBJ:=$(patsubst %.c, %.c.o, $(CSRC))

CFLAGS+=-c -MMD -Og -g -Wall -Werror -Wextra -I.
LFLAGS+=

BIN=turbo

%.c.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN): $(COBJ)
	$(CC) $(LFLAGS) $(COBJ) -o $@

.PHONY: all run debug clean

all: $(BIN)

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	gdb $(BIN)

clean:
	rm -f *.c.o *.c.d $(BIN)

-include *.c.d
