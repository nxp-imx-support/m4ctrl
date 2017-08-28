SOC ?= IMX8QM
CFLAGS ?= -Wall -g -D$(SOC)
CROSS_COMPILE ?=aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
INCLUDES := -I../include -I../include/platform
.PHONY: all clean

TARGET := m4ctrl

ifeq (IMX8, $(shell echo $(SOC) | cut -c1-4))
SOURCES += platform/imx8.c
endif


SOURCES += $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@
%.o: %.c
	$(CC) -D$(SOC) -c $< -o $@
clean:
	find . -name *.o | xargs rm -f
	-rm -f *~ m4ctrl
