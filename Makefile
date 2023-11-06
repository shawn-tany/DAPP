# DAPP installation package directory
PACKAGE_DIR = $(DAPP_ROOT_PATH)/package

# DAPP installation package
PACKAGE = dapp-install-package.tar.gz

# Common variable
# DAPP tools directory
TOOLS_DIR = $(DAPP_ROOT_PATH)/tools

# DAPP build directory
BUILD_DIR = $(DAPP_ROOT_PATH)/build

# make flags
MAKE_FLAGS = --no-print-directory 

.PHONY : ALL clean install uninstall

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please execute configure before the first compilation")
endif

ifneq ($(DAPP_ROOT_PATH),$(PWD))
$(error "The compilation area has changed, please re execute configure")
endif

ifeq ($(DAPP_INSTALL_PATH),)
$(error "Please execute configure before install")
endif

ALL :
	@echo "  build DAPP..."
	@(make $(MAKE_FLAGS) -f $(DAPP_ROOT_PATH)/source/Makefile) || (exit 3)
	@(make $(MAKE_FLAGS) -f $(DAPP_ROOT_PATH)/tools/Makefile)  || (exit 3)
	@echo $(BUILD_VERSION) > $(BUILD_DIR)/version
	@echo "  build DAPP success"

clean :
	@make $(MAKE_FLAGS) -f source/Makefile clean
	@make $(MAKE_FLAGS) -f tools/Makefile clean
	@rm $(BUILD_DIR) -rf

install : install_check
	@echo " install DAPP..."
	@# update package
	@cp $(BUILD_DIR)/app/* $(PACKAGE_DIR)/app -rf
	@cp $(BUILD_DIR)/version $(PACKAGE_DIR)/ -rf
	@cp $(TOOLS_DIR)/script/* $(PACKAGE_DIR)/tools -rf

	@# Create Package
	@rm $(PACKAGE_DIR)/$(PACKAGE) -rf
	@tar -zcPf $(PACKAGE_DIR)/$(PACKAGE) $(PACKAGE_DIR)/*

	@# Install to system directory
	@cp $(PACKAGE_DIR)/* $(DAPP_INSTALL_PATH) -rf
	@echo " install success!"

uninstall :
	@echo " uninstall DAPP..."
	@if [ -d "$(DAPP_INSTALL_PATH)" ]; then rm $(DAPP_INSTALL_PATH) -rf; fi
	@echo " uninstall success!"

include $(DAPP_ROOT_PATH)/env.mk
