SOC ?= IMX8QM
CROSS_COMPILE ?= aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
armv7_socs := IMX6SX IMX7S IMX7D
armv8_socs := IMX8QM IMX8QXP
supported_socs := $(armv7_socs) $(armv8_socs)

targets := user
ifneq ($(filter $(SOC), $(supported_socs)),$(SOC))
    $(error Undefined soc type: '$(SOC)', use one from: $(supported_socs))
endif

ifeq ($(filter $(SOC), $(armv8_socs)),$(SOC))
targets += kernel
endif

clean_targets = $(addprefix clean_,$(targets))
define generate_build_rule
$(1):
	make -C ./$(1) SOC=$(2) CROSS_COMPILE=$(3) CC="$(4)" CFLAGS="$(5)"
endef
define generate_clean_rule
clean_$(1):
	make -C ./$(1) clean
endef
.PHONY: all clean $(targets) $(clean_targets)
all: $(targets)
clean: $(clean_targets)

$(foreach target,$(targets),$(eval $(call generate_build_rule,$(target),$(SOC),$(CROSS_COMPILE),$(CC),$(CFLAGS))))

$(foreach target,$(targets),$(eval $(call generate_clean_rule,$(target))))
