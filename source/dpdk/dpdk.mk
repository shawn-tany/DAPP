DPDK_DIR = $(SOURCE_DIR)/dpdk

SRCS += $(DPDK_DIR)/dpdk.c
SRCS += $(DPDK_DIR)/session.c

CFLAGS += -I $(DPDK_DIR)