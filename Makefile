ROOT_DIR = $(PWD)
SYS_SBIN_DIR = /usr/sbin
SYS_INSTALL_DIR = /etc/DAPP

include source/Makefile

install :
	@echo " install DAPP"
	@if [ ! -d "$(SYS_INSTALL_DIR)" ]; then mkdir $(SYS_INSTALL_DIR); fi
	@cp $(PACKAGE_DIR)/config $(SYS_INSTALL_DIR) -rf
	@cp $(PACKAGE_DIR)/bin $(SYS_INSTALL_DIR) -rf
	@cp $(PACKAGE_DIR)/bin/$(APP) $(SYS_SBIN_DIR) -rf
	@echo " install finish!"