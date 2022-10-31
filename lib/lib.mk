LIB_DIR = $(ROOT_DIR)/lib
LIB_DPDK_DIR = $(LIB_DIR)/dpdk
LIB_JANSSON_DIR = $(LIB_DIR)/jansson

LIBS += $(LIB_DPDK_DIR)/lib/libdpdk.a
LIBS += $(LIB_JANSSON_DIR)/lib/libjansson.a

CFLAGS += -I $(LIB_DPDK_DIR)/include
CFLAGS += -I $(LIB_JANSSON_DIR)/include
CFLAGS += -lpthread -lnuma -ldl