DPDK_API_DIR = $(SOURCE_DIR)/dpdk-api

SRCS += $(DPDK_API_DIR)/dpdk.c

CFLAGS += -I $(DPDK_API_DIR)