ifndef TARGET
TARGET=z1
endif
SMALL=1

all: $(CONTIKI_PROJECT)
CONTIKI = /home/user/contiki
#CONTIKI = /home/tanguy/contiki
CONTIKI_WITH_RIME = 1
#CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include
