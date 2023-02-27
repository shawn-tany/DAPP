DPDK_DIR = $(SOURCE_DIR)/dpdk

SRCS += $(DPDK_DIR)/dpdk.c
SRCS += $(DPDK_DIR)/session.c $(COMMON_DIR)/dapp_list.c

CFLAGS += -I $(DPDK_DIR) -I $(COMMON_DIR)