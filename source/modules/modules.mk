MODULES_DIR = $(SOURCE_DIR)/modules

SRCS += $(MODULES_DIR)/modules.c
SRCS += $(MODULES_DIR)/config.c

CFLAGS += -I $(MODULES_DIR)

include $(MODULES_DIR)/control/control.mk
include $(MODULES_DIR)/port/port.mk
include $(MODULES_DIR)/files/files.mk
include $(MODULES_DIR)/flows/flows.mk
include $(MODULES_DIR)/protocol/protocol.mk
include $(MODULES_DIR)/restore/restore.mk
include $(MODULES_DIR)/rule/rule.mk
