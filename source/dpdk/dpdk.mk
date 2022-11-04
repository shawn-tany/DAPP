DPDK_DIR = $(SOURCE_DIR)/dpdk

SRCS += $(DPDK_DIR)/dpdk.c

CFLAGS += -I $(DPDK_DIR)