PROJECT = awesomesauce
BUILD_DIR = bin

SHARED_DIR = ../my-common-code
CFILES = my-project.c adc.c
# CFILES += api.c
# AFILES += api-asm.S

# TODO - you will need to edit these two lines!
DEVICE=lm4f120xl
OOCD_FILE = board/lm4f120xl.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
# OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
