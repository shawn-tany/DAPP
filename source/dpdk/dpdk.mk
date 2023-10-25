DPDK_DIR = $(SOURCE_DIR)/dpdk

SRCS += $(DPDK_DIR)/dpdk.c
SRCS += $(DPDK_DIR)/session.c 

SRCS += $(COMPONENTS_DIR)/structure/dapp_list.c

CFLAGS += -I $(DPDK_DIR) -I $(COMPONENTS_DIR)
