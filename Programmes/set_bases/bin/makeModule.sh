#! /bin/sh
#----- DEB FOR LINUX --------
  QTDIR="/home/rolandsevin/Documents/medintux-amd64/qt3_l64"
 export QTDIR
  PATH=$PATH":/home/rolandsevin/Documents/medintux-amd64/qt3_l64/bin"
 export PATH
#----- END FOR LINUX --------
QMAKESPEC=$QTDIR/mkspecs/linux-g++
export QMAKESPEC
cd '/home/rolandsevin/Documents/medintux-amd64/Programmes/set_bases/bin/'
./MakeAllMac.sh 'set_bases' '/home/rolandsevin/Documents/medintux-amd64/qt3_l64'