APPLICATION = activity-tracker
BOARD ?= 0sensorboard

USEMODULE += radio
EXTERNAL_MODULE_DIRS += $(CURDIR)/radio

USEMODULE += xtimer

FEATURES_PROVIDED += periph_adc
FEATURES_REQUIRED += periph_gpio
FEATURES_PROVIDED += periph_rtc
FEATURES_PROVIDED += periph_spi
FEATURES_PROVIDED += periph_timer
FEATURES_PROVIDED += periph_uart

FEATURES_REQUIRED += cpp # basic C++ support
FEATURES_REQUIRED += libstdcpp # libstdc++ support (for #include <cstdio>)

RIOTBASE ?= $(CURDIR)/../riot
include $(RIOTBASE)/Makefile.include
