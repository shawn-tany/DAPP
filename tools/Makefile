# compile tool
CC = gcc

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

# all directory are defined in here
TOOLS_DIR  = $(DAPP_ROOT_PATH)/tools
BUILD_DIR  = $(DAPP_ROOT_PATH)/build
COMMON_DIR = $(DAPP_ROOT_PATH)/source/common
LIB_DIR    = $(DAPP_ROOT_PATH)/lib

TOOL_COMPILE_DIRS  = $(TOOLS_DIR)/cli
TOOL_COMPILE_DIRS += $(TOOLS_DIR)/dir
TOOL_COMPILE_DIRS += $(TOOLS_DIR)/deamon 
TOOL_COMPILE_DIRS += $(TOOLS_DIR)/match
TOOL_COMPILE_DIRS += $(TOOLS_DIR)/replay

ALL :
	@# check environment variable
	@echo " make TOOLS..."
	@# create directory
	@if [ ! -d "$(BUILD_DIR)" ]; then mkdir $(BUILD_DIR); fi 
	@if [ ! -d "$(BUILD_DIR)/bin" ]; then mkdir $(BUILD_DIR)/bin; fi

	@for dir in $(TOOL_COMPILE_DIRS);			\
	do 							\
	   make --no-print-directory -f $$dir/Makefile;		\
	done;

	@# compile
	@echo " make success!"

clean :
	@for dir in $(TOOL_COMPILE_DIRS);			\
	do 							\
	   make --no-print-directory -f $$dir/Makefile clean;	\
	done;

	
