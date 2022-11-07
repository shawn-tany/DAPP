LIB_DIR = $(ROOT_DIR)/lib

LIB_DPDK_DIR = $(LIB_DIR)/dpdk
LIB_JANSSON_DIR = $(LIB_DIR)/jansson
LIB_HYPERSCAN_DIR = $(LIB_DIR)/hyperscan

CFLAGS += -I $(LIB_DPDK_DIR)/include
CFLAGS += -I $(LIB_JANSSON_DIR)/include
CFLAGS += -lpthread -lnuma -ldl -march=native

CFLAGS += -L $(LIB_JANSSON_DIR)/lib -ljansson -Wl,-rpath=$(LIB_JANSSON_DIR)/lib
CFLAGS += -L $(LIB_HYPERSCAN_DIR)/lib -lhs -Wl,-rpath=$(LIB_HYPERSCAN_DIR)/lib
CFLAGS += -L $(LIB_DPDK_DIR)/lib -ldpdk -Wl,-rpath=$(LIB_DPDK_DIR)/lib

# Ethernet devices can also be found without using this compilation option
#CFLAGS += -L $(LIB_DPDK_DIR)/lib -lrte_pmd_e1000 -Wl,-rpath=$(LIB_DPDK_DIR)/lib