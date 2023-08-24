# compile tool
CC = gcc

# binary name
APP = dapp

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

# all directory are defined in here
SOURCE_DIR = $(DAPP_ROOT_PATH)/source
BUILD_DIR  = $(DAPP_ROOT_PATH)/build

# all source are stored in SRCS
SRCS = $(SOURCE_DIR)/main.c

# all library are stored in LIBS

CFLAGS += -g
CFLAGS += -D DAPP_DEBUG
CFLAGS += -D DAPP_CONFIG_PATH=\"$(DAPP_INSTALL_PATH)/config\"
CFLAGS += -D DAPP_CACHE_PATH=\"$(DAPP_INSTALL_PATH)/cache\"
CFLAGS += -D DAPP_BUILD_VERSION=\"$(BUILD_VERSION)\"
CFLAGS += -O0

ALL :
	@echo " make DAPP..."
	
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi
	
	@# compile
	@$(CC) $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/bin/$(APP) $(LIBS)
	@echo " make success!"

clean :
	@echo " clean DAPP..."
	@rm $(BUILD_DIR)/bin/$(APP) -rf
	@echo " clean success!"

include $(DAPP_ROOT_PATH)/env.mk
include $(DAPP_ROOT_PATH)/lib/lib.mk
include $(SOURCE_DIR)/dpdk/dpdk.mk
include $(SOURCE_DIR)/common/common.mk
include $(SOURCE_DIR)/components/components.mk
include $(SOURCE_DIR)/config/config.mk
include $(SOURCE_DIR)/modules/modules.mk

