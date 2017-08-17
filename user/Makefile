CFLAGS = -Wall -g
CROSS_COMPILE?=aarch64-linux-gnu-
CC:=$(CROSS_COMPILE)gcc 
.PHONY: all clean

all: m4ctrl

clean:
	-rm -f *~ *.o m4ctrl
