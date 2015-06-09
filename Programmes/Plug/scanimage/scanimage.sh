#!/bin/bash
scanimage  --mode "Black & White" > ../../tmp/test_scanimage.pnm
convert ../../tmp/test_scanimage.pnm ../../tmp/test_scanimage.jpg
