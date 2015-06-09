#!/bin/bash

rm ~/Images/Webcam/mon_image
mavariable=$(/bin/ls ~/Images/Webcam/ | tail -1)
mv -f ~/Images/Webcam/"$mavariable" ~/Images/Webcam/mon_image
cp ~/Images/Webcam/mon_image $1
