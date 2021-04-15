# Set the name of your application:
APPLICATION = activity-tracker

# If no BOARD is found in the environment, use this default:
BOARD ?= stm32f0discovery

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/arteeh/git/activitytracker/riot

include $(RIOTBASE)/Makefile.include
