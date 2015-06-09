#!/bin/bash
# file : listdevice.sh
FILE_OUT=$1
scanimage -f%d"-+-">"$FILE_OUT"

