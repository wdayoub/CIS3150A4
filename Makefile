CC = gcc
CFLAGS = -o
LIBS = -lncurses
TARGETS = a4 a4ng
SRCS = a4.c

all: $(TARGETS)


a4: $(SRCS)
	$(CC) $(CFLAGS) a4 $(SRCS) $(LIBS)


a4ng: $(SRCS)
	$(CC) $(CFLAGS) a4ng $(SRCS) -DNOGRAPHICS

clean:
	rm -f $(TARGETS)

.PHONY: all clean