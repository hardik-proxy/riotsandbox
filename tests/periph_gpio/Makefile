# name of your application
export APPLICATION = test_periph_gpio

# If no BOARD is found in the environment, use this default:
export BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
export RIOTBASE ?= $(CURDIR)/../../../RIOT

include $(RIOTBASE)/tests/Makefile.tests_common

DISABLE_MODULE += auto_init

include $(RIOTBASE)/Makefile.include
