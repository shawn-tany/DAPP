#! /bin/bash

result1 = $(lsmod |grep igb_uio)
result2 = $(modinfo /etc/DAPP/kmod/igb_uio.ko |grep depends |grep uio)

# step 1 install igb_uio
if [[ "result1" == "" ]]; then
	if [[ "result2" != "" ]]; then 
		modprobe uio;
	fi
	insmod /etc/DAPP/kmod/igb_uio.ko;
fi

# step 2 bind ethernet device
. dpdk-devbind.py --bind=igb_uio $1 $2

