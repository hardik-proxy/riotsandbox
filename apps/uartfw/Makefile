# name of your application
APPLICATION = uartfw

# If no BOARD is found in the environment, use this default:
BOARD ?= mbed_lpc1768

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Uncomment this to enable scheduler statistics for ps:
#CFLAGS += -DSCHEDSTATISTICS

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:

FEATURES_REQUIRED += periph_uart

include $(RIOTBASE)/Makefile.include
