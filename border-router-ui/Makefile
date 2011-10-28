CONTIKI_PROJECT = border-router
all: $(CONTIKI_PROJECT)

CONTIKI = ../../

WITH_UIP6 ?= 1
UIP_CONF_IPV6 ?= 1

CFLAGS += -DUIP_CONF_IPV6_RPL
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

## Linker optimizations
SMALL = 1

#APPS += webbrowser
PROJECT_SOURCEFILES += slip-bridge.c #wget.c

## Simple built-in webserver is the default.
## Override with make WITH_WEBSERVER=0 for no webserver.
## WITH_WEBSERVER=webserver-name will use /apps/webserver-name if it can be
## found in the /apps, /platform/$(TARGET)/apps/, or current directory (in that order).
##  WITH_WEBSERVER=webserver for /apps/webserver
##  WITH_WEBSERVER=raven-webserver for /platform/avr-raven/apps/raven-webserver/
## make clean before changing webservers!

WITH_WEBSERVER ?= 1

ifeq ($(WITH_WEBSERVER), 1)
  CFLAGS += -DWEBSERVER=1
  PROJECT_SOURCEFILES += httpd-simple.c
else ifneq ($(WITH_WEBSERVER), 0)
  APPS += $(WITH_WEBSERVER)
  CFLAGS += -DWEBSERVER=2
endif

## Note /apps/webserver contains a 2500 byte style sheet which is a severe test
## of the slip connection. Large MSS together with low baud rates without flow
## control will overrun the transmit buffer when the style sheet is requested.


## This has to be set before Makefile.options is included!
TERM_TITLE = "Contiki RPL Border Router Console ($(NODE_ALIAS))"

include $(CONTIKI)/Makefile.include
include $(CONTIKI)/Makefile.options

$(CONTIKI)/tools/tunslip6: $(CONTIKI)/tools/tunslip6.c
	(cd $(CONTIKI)/tools && $(MAKE) tunslip6)

connect-router: $(REQUIRE_TUNSLIP)
	$(SU_COMMAND) $(TUNSLIP_EXECUTE)

connect-router-xterm: $(REQUIRE_TUNSLIP)
	$(TERM_EXECUTE) $(TUNSLIP_EXECUTE)

#TODO: add connect-router-xscreen here!
#	see TODO in Makefile.mc1322x :)
#	we will need Makefile.utils

connect-router-cooja: $(REQUIRE_TUNSLIP)
	$(TUNSLIP_EXECUTE) $(TUNSLIP_OPTIONS) \
		-a localhost $(TUNSLIP_ADDRESS)
