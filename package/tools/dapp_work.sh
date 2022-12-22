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
		echo "work mode online"
		$DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=$UIO_DRV $PORT_BIND1 $PORT_BIND2 >/dev/null 2>&1
	fi
	
	$DAPP_INSTALL_PATH/bin/dapp &
	echo "dapp running"
}

function stop {
	killall -2 dapp > /dev/null 2>&1

	$DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=$NET_DRV $PORT_BIND1 $PORT_BIND2 > /dev/null 2>&1

	echo "dapp stop"
}

function restart {
	stop
	sleep 5
	start $1
}

function help
{
	echo "OPTIONS :"
	echo "	start [online|offline]"
	echo "	restart [online|offline]"
	echo "	online"
	echo "	offline"
	echo "	stop"
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
elif [ "$1" = "help" ]
then
	help
else
	echo "invalid paramter $1"
	help
fi
