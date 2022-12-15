 CONTROL_DIR = $(MODULES_DIR)/control

SRCS += $(CONTROL_DIR)/control.c
SRCS += $(CONTROL_DIR)/statistics.c
SRCS += $(CONTROL_DIR)/cli_server.c

CFLAGS += -I $(CONTROL_DIR)