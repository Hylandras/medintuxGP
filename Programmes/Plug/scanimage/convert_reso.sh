#!/bin/bash

ERRORLOG="convert_reso.log"  # file where STDERR goes


RESO=$1            # resolution in dpi
SRC_FILE=$2
DST_FILE=$3        
ERRORLOG=$4        # file where STDERR goes

# redirect STDOUT to FILE_OUT
exec 1>>$FILE_OUT
# redirect STDERR to ERRORLOG
exec 2>>$ERRORLOG

FILE_OUT="convert_reso.out"

if [ $5 == 'debug' ]; then
echo "=========================================">&2
echo "            medintux convert_reso">&2
echo "=========================================">&2
echo "------------ arguments-----------------">&2
echo $1>&2
echo $2>&2
echo $3>&2
echo $4>&2
echo "------------ fichiers-----------------">&2
echo "Fichier src : "$SRC_FILE>&2
echo "Fichier dst : "$DST_FILE>&2
echo "Resolution  : "$RESO>&2
echo "File Log    : "$ERRORLOG>&2
echo "--------------------------------------">&2
fi

convert -compress none -depth 8 -resample $RESOx$RESO $SRC_FILE $DST_FILE 1>&2
