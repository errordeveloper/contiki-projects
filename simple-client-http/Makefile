CONTIKI_PROJECT = psock-client
CONTIKI = ../../

ifeq ($(ROUTER_EMBEDDED),1)

  include $(CONTIKI)/examples/ipv6/rpl-border-router/Makefile

else

PROJECT_SOURCEFILES += uart2.c #$(MIDI_LIB_SOURCEFILES) # uart2-midi.c

all: $(CONTIKI_PROJECT)

UIP_CONF_IPV6=1

include $(CONTIKI)/Makefile.include

router:
	cd $(CONTIKI)/examples/ipv6/rpl-border-router/ \
	&& make border-router.load \
	&& make connect-router-xterm
endif
