SOC ?= IMX8QM
family := $(shell echo $(SOC) | cut -c1-4)
CFLAGS += -Wall -g -D$(SOC)
CROSS_COMPILE ?=aarch64-linux-gnu-
CC ?= $(CROSS_COMPILE)gcc
INCLUDES := -I../include -I../include/platform
.PHONY: all clean

TARGET := m4ctrl

ifeq ($(family),IMX8)
SOURCES += platform/imx8.c
else
ifneq ($(filter $(family), IMX6 IMX7),)
SOURCES += platform/imx_v6_v7.c
else
$(error "Undefined SOC!")
endif
endif

SOURCES += $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	find . -name "*.o" -delete
	-rm -f *~ m4ctrl
