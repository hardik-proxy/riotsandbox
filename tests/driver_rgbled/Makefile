# name of your application
export APPLICATION = test_rgbled

# If no BOARD is found in the environment, use this default:
export BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
export RIOTBASE ?= $(CURDIR)/../../../RIOT


USEMODULE += vtimer
USEMODULE += color
USEMODULE += rgbled

include $(RIOTBASE)/Makefile.include
