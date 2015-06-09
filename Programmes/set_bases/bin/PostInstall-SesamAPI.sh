#! /bin/sh
echo "."
echo " ... Modifier droits de l'API Sesam-Vitale ....."
chmod -R 777 /usr/local/sesam
chmod -R 777 /usr/local/galss
chmod -R 777 /usr/local/galsssrv
ln -s /usr/lib/libssl.so.0.9.8 /usr/lib/libssl.so.4
ln -s /usr/lib/libstdc++-3-libc6.2-2-2.10.0.so /usr/lib/libstdc++-libc6.1-1.so.2
chmod 777 /dev/ttyS0
chmod 777 /dev/ttyS1
ldconfig
