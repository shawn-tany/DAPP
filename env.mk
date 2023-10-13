BUILD_VERSION = $(shell $(ROOT_DIR)/version.sh)

CFLAGS += -Wall -Werror
CFLAGS += -g
CFLAGS += -D DAPP_BUILD_VERSION=\"$(BUILD_VERSION)\"