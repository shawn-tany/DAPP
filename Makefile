# DAPP installation package directory
PACKAGE_DIR = $(DAPP_ROOT_PATH)/package

# DAPP installation package
PACKAGE = dapp-install-package.tar.gz

# Common variable
# DAPP tools directory
TOOLS_DIR = $(DAPP_ROOT_PATH)/tools

# DAPP build directory
BUILD_DIR = $(DAPP_ROOT_PATH)/build

.PHONY : ALL clean install uninstall

ifeq ($(DAPP_ROOT_PATH),)
$(error "Please define DAPP_ROOT_PATH environment variable")
endif

ifeq ($(DAPP_INSTALL_PATH),)
$(error "Please define DAPP_INSTALL_PATH environment variable")
endif

ALL :
	@make --no-print-directory -f $(DAPP_ROOT_PATH)/source/Makefile
	@make --no-print-directory -f $(DAPP_ROOT_PATH)/tools/Makefile

clean :
	@make --no-print-directory -f source/Makefile clean
	@make --no-print-directory -f tools/Makefile clean

install :
	@echo " install DAPP..."
	@if [ -d "$(DAPP_INSTALL_PATH_OLD)" ]; then rm $(DAPP_INSTALL_PATH_OLD) -rf; fi
	@if [ ! -d "$(DAPP_INSTALL_PATH)" ]; then mkdir $(DAPP_INSTALL_PATH); fi
	@if [ ! -d "$(DAPP_INSTALL_PATH)/cache" ]; then mkdir $(DAPP_INSTALL_PATH)/cache; fi
	@if [ ! -d "$(PACKAGE_DIR)" ]; then mkdir $(PACKAGE_DIR); fi
	@if [ ! -d "$(PACKAGE_DIR)/bin" ]; then mkdir $(PACKAGE_DIR)/bin; fi
	@if [ ! -d "$(PACKAGE_DIR)/tools" ]; then mkdir $(PACKAGE_DIR)/tools; fi

	@# update package
	@cp $(BUILD_DIR)/bin/* $(PACKAGE_DIR)/bin -rf
	@cp $(TOOLS_DIR)/*.sh $(PACKAGE_DIR)/tools -rf
	@cp $(TOOLS_DIR)/*.py $(PACKAGE_DIR)/tools -rf

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
