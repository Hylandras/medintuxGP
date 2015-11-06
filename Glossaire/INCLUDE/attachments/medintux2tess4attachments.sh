#!/bin/bash
##############################################################################
# medintux2tess4attachments
##############################################################################
# medintux2tess4attachments is a TesseractOCR wrapper to be able to use tesseract with
# MedinTux
# This work is based on the original medintux2tess

ERRORLOG="medintux2tess4attachments--"$(date +"%Y-%m-%d-%H-%M")".log" # file where STDERR goes
USE_WILDCARD=$1
DO_OCR=$5
TES_LANG=$2
TEMP_DIR=$3
LOG_DIR=$6

#............... redirect STDERR to ERRORLOG ...............
exec 2>>$LOG_DIR$ERRORLOG

if [ $4 = 'debug' ]; then
	echo "=========================================">&2
	echo " medintux2tess4attachments">&2
	echo "=========================================">&2
	echo "------------ Arguments -------------------">&2
	echo "Wildcard : "$USE_WILDCARD>&2
	echo "Language : "$TES_LANG>&2
	echo "Do OCR : "$DO_OCR>&2
	echo "Temp Dir : "$TEMP_DIR>&2
	echo "Log DIR : "$LOG_DIR>&2
	echo "-----------------------------------------">&2
fi

#............... cleaning for the new output ...............
#Usually ocr_pdf.txt is destroyed during the import but in case of bug
#we perform this action so the validating test occuring after this script
#won't return any false positive
rm "$TEMP_DIR"ocr_pdf.txt

#............... start OCR (tesseract expands output with *.txt) ...............
#First we copy preview_pdf_00 to be sure to keep a preview file
cp "$TEMP_DIR"preview_pdf_00.png "$TEMP_DIR"preview_pdf.png

#if DO_OCR is not equal to "SI" then we only delete temp files
if [ $DO_OCR = 'YES' ]; then
	if [ $USE_WILDCARD = 'YES' ]; then #if YES then our PDF has multiple pages
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
else
	if [ $USE_WILDCARD = 'YES' ]; then
			rm "$TEMP_DIR"ocr_pdf.multi*.txt
	else
			rm "$TEMP_DIR"preview_pdf_00.png
	fi
fi
