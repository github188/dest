#!/bin/bash
shopt -s  extglob

if  [ ! -d /opt ]; then
    mkdir /opt
fi
if  [ ! -d /opt/mcu ]; then
    mkdir /opt/mcu
fi
if  [ ! -d /opt/mcu/api ]; then
    mkdir /opt/mcu/api
fi
if  [ ! -d /opt/mcu/api/config ]; then
	mkdir /opt/mcu/api/config
fi
if  [ ! -f /opt/mcu/api/instab ]; then
    touch /opt/mcu/api/instab
fi

num=`grep "inistalled_api" /opt/mcu/api/instab | wc -l`
if [ "$num" = "0" ]; then
  echo Installing Kedacom Video Conference API, please wait...
else
  echo Updating Kedacom Video Conference API, please wait....
# remove all the files expect configs 
  rm -rf /opt/mcu/api/!(config)
fi

sed '1,/^#here/d' $0 > /opt/mcu/api.tar
tar xvf /opt/mcu/api.tar -C /opt/mcu/api
rm -rf /opt/mcu/api.tar

cd /opt/mcu/api

tar zxvf ./env.tgz
tar zxvf ./src.tgz

rm -rf env.tgz
rm -rf src.tgz

chmod a+x ./src/start.sh

#./src/start.sh &

echo Installation completed at `pwd`.

echo 'inistalled_api' >> /opt/mcu/api/instab
exit
#here
