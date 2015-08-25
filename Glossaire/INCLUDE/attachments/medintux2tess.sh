#!/bin/bash
##############################################################################
# medintux2tess4attachments
##############################################################################
# medintux2tess4attachments is a TesseractOCR wrapper to be able to use tesseract with
# MedinTux

ERRORLOG="medintux2tess--"$(date +"%Y-%m-%d-%H-%M")".log" # file where STDERR goes
TES_LANG=$2
TEMP_DIR=$3

#............... redirect STDERR to ERRORLOG ...............
exec 2>>$TEMP_DIR$ERRORLOG

if [ $4 = 'debug' ]; then
echo "=========================================">&2
echo " medintux2tess">&2
echo "=========================================">&2
echo "------------ arguments-------------------">&2
echo $1>&2
echo $2>&2
echo $3>&2
echo "------------ fichiers--------------------">&2
echo "Rep. Temp : "$TEMP_DIR>&2
echo "Language : "$TES_LANG>&2
echo "-----------------------------------------">&2
fi

#............... cleaning for the new output ...............
rm "$TEMP_DIR"ocr_pdf.txt

#............... start OCR (tesseract expands output with *.txt) ...............
if [ $1 = 'YES' ]; then
		c=000
		for i in "$TEMP_DIR"preview_pdf_*.png
			do
				tesseract $i "$TEMP_DIR"ocr_pdf.multi$c $TES_LANG >&2
				c=`printf "%03d" $((c+1))`
				rm $i
			done
		cat "$TEMP_DIR"ocr_pdf.multi*.txt > "$TEMP_DIR"ocr_pdf.txt
		rm "$TEMP_DIR"ocr_pdf.multi*.txt
	else
		tesseract "$TEMP_DIR"preview_pdf_00.png "$TEMP_DIR"ocr_pdf $TES_LANG >&2
		rm "$TEMP_DIR"preview_pdf_00.png
	fi
