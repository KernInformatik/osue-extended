.PHONY: all clean

defs = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
verbose = -std=c99 -pedantic -g3 -Wall -Wextra -Wwrite-strings -Wconversion -Wformat=2 -Warray-bounds -Wstack-protector -Wshadow $(defs)
library = -lpthread -pthread -lrt

TARGETS = supervisor generator
OBJS = supervisor.o generator.o
DEPS = common.h

all: $(TARGETS)

supervisor: supervisor.o
	gcc $(verbose) -o $@ $^ $(library)

generator: generator.o
	gcc $(verbose) -o $@ $^ $(library)

%.o: %.c $(DEPS)
	gcc $(verbose) -c $< -o $@

clean:
	rm -f $(TARGETS) $(OBJS)