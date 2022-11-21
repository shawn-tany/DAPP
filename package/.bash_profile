# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

DAPP_ROOT_PATH=$(pwd)
export DAPP_ROOT_PATH

DAPP_INSTALL_PATH=/etc/DAPP

DAPP_RUNNING_PATH=$DAPP_INSTALL_PATH/bin:$DAPP_INSTALL_PATH/tools

PATH=$PATH:$HOME/.local/bin:$HOME/bin:$DAPP_RUNNING_PATH
export PATH
