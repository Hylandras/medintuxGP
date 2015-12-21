#!/bin/bash
##############################################################################
#medintux2tess4attachments
##############################################################################
#medintux2tess4attachments is a TesseractOCR wrapper to be able to use tesseract with
# MedinTux
# This work is based on the original medintux2tess

ERRORLOG="medintux2tess4pdfattachments--"$(date +"%Y-%m-%d-%H-%M")".log" # file where STDERR goes
PAT_DIR=$1 #It's the directory where files are finally stored
TES_LANG=$2 #Language setting for Tesseract
TEMP_DIR=$3 #Directory of temporary files
#the fourth argument is not here because we only need it once for debug
DO_OCR=$5 #PDF only : if NO then we only delete temporary preview files
LOG_DIR=$6 #Directory of Log Files
FILE_NAME=$7 #File name is useful for images where extension can change, when we call the script for a PDF we give "pdf" as value so we can easily test if we need to run this script for a simple picture or a PDF file without needing a tenth variable
LAST_NAME=$8 #It's the final name (with extension) of the file for which we run an OCR

#............... redirect STDERR to ERRORLOG ...............
exec 2>>$LOG_DIR$ERRORLOG

if [ $4 = 'debug' ]; then
	echo "=========================================">&2
	echo "medintux2tess4attachments">&2
	echo "=========================================">&2
	echo "------------ Arguments -------------------">&2
	echo "Language : "$TES_LANG>&2
	echo "Do OCR : "$DO_OCR>&2
	echo "Temp Dir : "$TEMP_DIR>&2
	echo "Log DIR : "$LOG_DIR>&2
	echo "File name : "$FILE_NAME>&2
	echo "Final name : "$LAST_NAME>&2
	echo "Patient Dir : "$PAT_DIR>&2
	echo "-----------------------------------------">&2
fi

if [ $FILE_NAME = 'pdf' ]; then
			convert -density 300 -trim "$TEMP_DIR"include_pdf.tmp.pdf -quality 100 -sharpen 0x1.0 "$TEMP_DIR"preview_pdf_%02d.png

			#First we copy preview_pdf_00 to be sure to keep a preview file
			cp "$TEMP_DIR"preview_pdf_00.png "$TEMP_DIR"preview_pdf.png

			#............... start OCR (tesseract expands output with *.txt) ...............
			if [ $DO_OCR = 'YES' ]; then
				if [ -f "$TEMP_DIR"'preview_pdf_01.png' ]; then
						c=000
						for i in "$TEMP_DIR"preview_pdf_*.png
							do
								tesseract $i "$TEMP_DIR"ocr_pdf.multi$c $TES_LANG >&2
								c=`printf "%03d" $((c+1))`
								rm $i
							done
						cat "$TEMP_DIR"ocr_pdf.multi*.txt > "$PAT_DIR""$LAST_NAME".ocr.txt
						rm "$TEMP_DIR"ocr_pdf.multi*.txt
				else
						tesseract "$TEMP_DIR"preview_pdf_00.png "$PAT_DIR""$LAST_NAME".ocr $TES_LANG >&2
						rm "$TEMP_DIR"preview_pdf_00.png
				fi
		else
				for i in "$TEMP_DIR"preview_pdf_*.png
					do
						rm $i
					done
		fi
else
		tesseract "$TEMP_DIR""$FILE_NAME" "$PAT_DIR""$LAST_NAME".ocr $TES_LANG >&2
fi
