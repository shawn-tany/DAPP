CONFIG_DIR = $(SOURCE_DIR)/config

SRCS += $(CONFIG_DIR)/startup_conf.c

CFLAGS += -I $(CONFIG_DIR)