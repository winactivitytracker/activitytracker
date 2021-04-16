APPLICATION = activity-tracker
BOARD ?= stm32f0discovery
RIOTBASE ?= $(CURDIR)/../riot

USEMODULE += radio
EXTERNAL_MODULE_DIRS += $(CURDIR)/radio
USEMODULE += xtimer

FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_uart
FEATURES_REQUIRED += periph_spi
#FEATURES_REQUIRED += periph_i2c

FEATURES_REQUIRED += cpp # basic C++ support
FEATURES_REQUIRED += libstdcpp # libstdc++ support (for #include <cstdio>)

include $(RIOTBASE)/Makefile.include
