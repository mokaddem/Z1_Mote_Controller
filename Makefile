ifndef TARGET
TARGET=z1
endif

all: $(CONTIKI_PROJECT)
#CONTIKI = /home/user/contiki
CONTIKI = /home/tanguy/contiki
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
