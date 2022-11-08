# compile tool
CC = gcc

# binary name
APP = dapp
PACKAGE = dapp-install-package.tar.gz

# all directory are defined in here
ROOT_DIR ?= $(PWD)/..
SOURCE_DIR = $(ROOT_DIR)/source
BUILD_DIR = $(ROOT_DIR)/build
PACKAGE_DIR = $(ROOT_DIR)/package

# all source are stored in SRCS
SRCS = $(SOURCE_DIR)/main.c

# all library are stored in SRCS

CFLAGS += -g
#CFLAGS += -D DAPP_DEBUG=1
CFLAGS += -O0

ALL :
	@echo " make DAPP"
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi
	
	@# compile
	@$(CC) $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/bin/$(APP) $(LIBS)

	@# update package
	@cp $(BUILD_DIR)/bin/$(APP) $(PACKAGE_DIR)/bin
	@rm $(PACKAGE_DIR)/$(PACKAGE) -rf
	@tar -zcPf $(PACKAGE_DIR)/$(PACKAGE) $(PACKAGE_DIR)/*
	@echo " make finish"

clean :
	@echo " clean DAPP"
	@rm $(BUILD_DIR) -rf
	@echo " clean finish"

include $(ROOT_DIR)/lib/lib.mk
include $(SOURCE_DIR)/dpdk/dpdk.mk
include $(SOURCE_DIR)/common/common.mk
include $(SOURCE_DIR)/config/config.mk
include $(SOURCE_DIR)/modules/modules.mk

