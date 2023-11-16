bindaction()
{
    if [[ "$1" == "bind" ]];
    then 
        if [[ $3 == igbuio ]];
        then
            echo "bind igb_uio $2"
        elif [[ $3 == vfiopci ]];
        then
            echo "bind vfio-pci $2"
        fi
        
        nicname=`$DAPP_INSTALL_PATH/tools/dpdk-devbind.py -s |grep $2 |awk -F "if=" '{print $2}' |awk -F " " '{print $1}'`
        if [[ "${nicname}" == "" ]];
        then 
            echo "No such ethernet device ${nicname}!"
            return 0
        fi

        ifconfig ${nicname} down

        ethdriver=`ethtool -i ${nicname} |grep driver |awk -F ": " '{print $2}'`
        if [[ "${nicname}" != "" && ${ethdriver} != "" ]];
        then
            if [[ "$first" != "" ]]
            then
                echo $2=${ethdriver} >> $DAPP_INSTALL_PATH/ethdriver.list
            else
                echo $2=${ethdriver} > $DAPP_INSTALL_PATH/ethdriver.list
                first=yes
            fi
        fi

        if [[ $3 == igbuio ]];
        then
            driver_exist=`lsmod |grep igb_uio`;
            if [[ ! $driver_exist ]]
            then 
                modprobe uio
                insmod $DAPP_INSTALL_PATH/kmod/igb_uio.ko > /dev/null 2>&1
            fi

            $DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=igb_uio $2 > /dev/null 2>&1
        elif [[ $3 == vfiopci ]];
        then
            $DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=vfio-pci $2 > /dev/null 2>&1
        fi

    else
        if [ ! -e $DAPP_INSTALL_PATH/ethdriver.list ];
        then
            return 0
        fi

        while read -r ntdline
        do
            ntdname=`echo ${ntdline} |awk -F "=" '{print $1}'`
            if [[ ${ntdname} == $2 ]];
            then
                ntddriver=`echo ${ntdline} |awk -F ":" '{print $2}'`
                if [[ $3 == igbuio ]];
                then
                    echo "${nicname} unbind igb_uio $2, bind ${ntddriver}"
                elif [[ $3 == vfiopci ]];
                then
                    echo "${nicname} unbind vfio-pci $2, bind ${ntddriver}"
                fi 

                $DAPP_INSTALL_PATH/tools/dpdk-devbind.py --bind=e1000 ${igbdev} > /dev/null 2>&1
                
                break
            fi
        done < $DAPP_INSTALL_PATH/ethdriver.list
    fi
}

function bindctl() {

    unset igbdevs
    unset vfiodevs

    igbnum=0
    vfionum=0

    while read -r line 
    do
        igbline=`echo ${line} |grep "\[igb_uio\]={"`
        vfioline=`echo ${line} |grep "\[vfio_pci\]={"`

        if [[ "${igbline}" != "" ]];
        then
            igbdevs[${igbnum}]=`echo ${igbline} |awk -F "{" '{print $2}' |awk -F ", " '{print $1}'`
            igbnum=$(($igbnum+1))
            igbdevs[${igbnum}]=`echo ${igbline} |awk -F "}" '{print $1}' |awk -F ", " '{print $2}'`
            igbnum=$(($igbnum+1))
        elif [[ "${vfioline}" != "" ]];
        then 
            vfiodevs[${vfionum}]=`echo ${vfioline} |awk -F "{" '{print $2}' |awk -F ", " '{print $1}'`
            vfionum=$(($vfionum+1))
            vfiodevs[${vfionum}]=`echo ${vfioline} |awk -F "}" '{print $1}' |awk -F ", " '{print $2}'`
            vfionum=$(($vfionum+1))
        fi
    done < ${DAPP_INSTALL_PATH}/config/eth.config

    for igbdev in ${igbdevs[*]}
    do
        bindaction $1 ${igbdev} igbuio
    done

    for vfiodev in ${vfiodevs[*]}
    do
        bindaction $1 ${vfiodev} vfiopci
    done

    if [[ $1 == unbind ]];
    then
        igb_uio_exist=`lsmod |grep igb_uio`;
        if [[ $igb_uio_exist ]]
        then 
            rmmod $DAPP_INSTALL_PATH/kmod/igb_uio.ko
        fi
    fi
}

function start {

	if [ "$1" = "offline" ]
	then
		echo "work mode offline"
                bindctl unbind
	else
		echo "work mode online"
                bindctl bind
	fi
	
	$DAPP_INSTALL_PATH/app/dapp_deamon $DAPP_INSTALL_PATH/app/dapp
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
	
        bindctl unbind	

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
	echo "	bind"
	echo "	unbind"
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
elif [ "$1" = "bind" ]
then
        bindctl bind
elif [ "$1" = "unbind" ]
then
        bindctl unbind
elif [ "$1" = "help" ]
then
	help
else
	echo "invalid paramter $1"
	help
fi
