all: midsn-nc

ifndef TARGET
TARGET=sky
endif

APPS+=powertrace

PROJECTDIRS+= drivers
PROJECTDIRS+= ioadapter
PROJECTDIRS+= nc-kernel-am
PROJECTDIRS+= nc-ginapp-cm
PROJECTDIRS+= nc-ginapp-gp
PROJECTDIRS+= nc-ginapp-aa

#Driver
CONTIKI_SOURCEFILES += communication_protocol.c so-driver.c

#IOAdapter
CONTIKI_SOURCEFILES += midsn_packet.c communication.c 

#NC-Kernel-AM
CONTIKI_SOURCEFILES += agent-manager.c

#NC-GinAPP-CM
CONTIKI_SOURCEFILES += config-manager.c

#NC-GinAPP-GP
CONTIKI_SOURCEFILES += events.c compute.c clockMidSN.c

#NC-GinAPP-AA
CONTIKI_SOURCEFILES += aquisition.c 


CONTIKI = ../..
include $(CONTIKI)/Makefile.include

