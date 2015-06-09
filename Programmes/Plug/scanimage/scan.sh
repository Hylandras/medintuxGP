#!/bin/bash
# file : scan.sh
DEVICE=$1
MODE=$2
RESO=$3
ROTATE=$4
FILE_OUT=$5
scanimage  --device-name="$DEVICE" --mode="$MODE" --resolution="$RESO" > ../../tmp/test_scanimage.pnm
convert -rotate "$ROTATE" '../../tmp/test_scanimage.pnm' "$FILE_OUT"".png"