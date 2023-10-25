PORT_BIND1="02:04.0"
PORT_BIND2="02:05.0"

NET_DRV=e1000
UIO_DRV=igb_uio

function start {

	if [ "$1" = "offline" ]
	then
		echo "work mode offline"
		$DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=$NET_DRV $PORT_BIND1 $PORT_BIND2 > /dev/null 2>&1
	else
		igb_uio_exist=`lsmod |grep igb_uio`;
	
		if [[ ! $igb_uio_exist ]]
		then 
			modprobe uio
			insmod $DAPP_INSTALL_PATH/kmod/igb_uio.ko
		fi
	
		echo "work mode online"
		$DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=$UIO_DRV $PORT_BIND1 $PORT_BIND2 > /dev/null 2>&1
	fi
	
	$DAPP_INSTALL_PATH/bin/dapp_deamon $DAPP_INSTALL_PATH/bin/dapp
	echo "dapp running"
}

function stop {
	deamon_pid_exist=`pidof dapp_deamon`;
	if [[ $deamon_pid_exist ]]
	then
	    killall -9 dapp_deamon > /dev/null 2>&1
	    echo "dapp_deamon stop"
	fi
	
	dapp_pid_exist=`pidof dapp`;
	if [[ $dapp_pid_exist ]]
	then
	    killall -2 dapp > /dev/null 2>&1
	    sleep 3
	    echo "dapp stop"
	fi
	
	$DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=$NET_DRV $PORT_BIND1 $PORT_BIND2 > /dev/null 2>&1
	
	igb_uio_exist=`lsmod |grep igb_uio`;
	
	if [[ $igb_uio_exist ]]
	then 
		rmmod $DAPP_INSTALL_PATH/kmod/igb_uio.ko
	fi
	
	if [[ ! $deamon_pid_exist && ! $dapp_pid_exist ]]
	then
	    echo "No DAPP program is running"
	fi
}

function restart {
	stop
	start $1
}

function version
{
	echo ""
	echo " DAPP version : $(cat ${DAPP_INSTALL_PATH}/version)"
	echo ""
}

function help
{
	echo "OPTIONS :"
	echo "	start    [online|offline]"
	echo "	restart  [online|offline]"
	echo "	online"
	echo "	offline"
	echo "	stop"
	echo "	version"
	echo "	help"
}

if [ "$1" = "start" -o "$1" = "" ]
then
	start $2
elif [ "$1" = "online" -o "$1" = "offline" ]
then
	start $1
elif [ "$1" = "" ]
then
	start online
elif [ "$1" = "stop" ]
then
	stop
elif [ "$1" = "restart" ]
then
	restart $2
elif [ "$1" = "version" ]
then
	version
elif [ "$1" = "help" ]
then
	help
else
	echo "invalid paramter $1"
	help
fi
