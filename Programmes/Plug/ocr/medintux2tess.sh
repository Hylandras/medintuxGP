#!/bin/bash
#
#
##############################################################################
#                                   medintux2tess
##############################################################################
# 
# medintux2tess is a TesseractOCR wrapper to be able to use tesseract with
# xsane and MedinTux
#
# ---------------------- MedinTux script exemple for use in MedinTux ---------
#                        to place in all place where MedinTuxScript's are
#                        possibles.
#                        (for script thereafter remove start line comment '# ')
# 
# $start_titleNumérisation et OCR avec Xsane et tesseract$end_title\
# {{::Intitule ( Importation Texte OCR ) }}\
# {{::exe_process (WaitEnd ,/usr/bin/xsane,--save,-N$ToAbsPath../../tmp/LastScanned.png)}}\
# {{::exe_process (WaitEnd,../../Plug/OCR/medintux2tess.sh,LastScanned.tif,LastScanned,-l fra,$ToAbsPath../../tmp$EndToAbsPath/,nodebug)}}\
# {{::import (../../tmp/LastScanned.txt , insert)}}

export TESSDATA_PREFIX=/usr/local/share/
ERRORLOG="medintux2tess.log"  # file where STDERR goes

TEMP_DIR=$4
# redirect STDOUT to FILE_OUT
exec 1>>$FILE_OUT

# redirect STDERR to ERRORLOG
exec 2>>$TEMP_DIR$ERRORLOG


TIF_FILE=$1
TXT_FILE=$2
TES_LANG=$3
FILE_OUT="medintux2tess.out"

 if [ $5 == 'debug' ]; then
echo "=========================================">&2
echo "            medintux2tess">&2
echo "=========================================">&2
echo "------------ arguments-----------------">&2
echo $1>&2
echo $2>&2
echo $3>&2
echo $4>&2
echo "------------ environnement-----------------">&2
echo "TESSDATA_PREFIX="$TESSDATA_PREFIX>&2
echo "------------ fichiers-----------------">&2
echo "Rep. Temp   : "$TEMP_DIR>&2
echo "Fichier tif : "$TEMP_DIR""$TIF_FILE>&2
echo "Fichier txt : "$TEMP_DIR""$TXT_FILE>&2
echo "Langage     : "$TES_LANG>&2
echo "--------------------------------------">&2
fi

# converting image into TIFF (ImageMagick)
convert  -compress none  "$TEMP_DIR""$TIF_FILE" "$TEMP_DIR""$TIF_FILE" 1>&2

# start OCR (tesseract expands output with *.txt)
tesseract  $TEMP_DIR$TIF_FILE $TEMP_DIR$TXT_FILE $TES_LANG 1>&2

# STDOUT scanned text => FILE_OUT
cat "$TEMP_DIR""$TXT_FILE".txt
