#
# Copyright 2017 NXP
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
SOC ?= IMX8QM
family := $(shell echo $(SOC) | cut -c1-4)
LCFLAGS := -Wall -g -D$(SOC)
CCFLAGS := $(CFLAGS) $(LCFLAGS)
CROSS_COMPILE ?=aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
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
	$(CC) $(CCFLAGS) -c $< -o $@
clean:
	find . -name "*.o" -delete
	-rm -f *~ m4ctrl
