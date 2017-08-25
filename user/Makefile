SOC ?= IMX8QM
CFLAGS ?= -Wall -g -D$(SOC)
CROSS_COMPILE ?=aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
INCLUDES := -I../include -I../include/platform
.PHONY: all clean

TARGET := m4ctrl


SOURCES += $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@
%.o: %.c
	$(CC) -D$(SOC) -c $< -o $@
clean:
	-rm -f *~ *.o m4ctrl
