LIB_DIR = $(ROOT_DIR)/lib

LIB_DPDK_DIR = $(LIB_DIR)/dpdk
LIB_JANSSON_DIR = $(LIB_DIR)/jansson
LIB_HYPERSCAN_DIR = $(LIB_DIR)/hyperscan

LIBS += $(LIB_DPDK_DIR)/lib/libdpdk.a
LIBS += $(LIB_JANSSON_DIR)/lib/libjansson.a

CFLAGS += -I $(LIB_DPDK_DIR)/include
CFLAGS += -I $(LIB_JANSSON_DIR)/include
CFLAGS += -lpthread -lnuma -ldl
CFLAGS += -L $(LIB_HYPERSCAN_DIR)/lib -lhs -Wl,-rpath=$(LIB_HYPERSCAN_DIR)/lib
CFLAGS += -march=native