#!/bin/bash
if test -f ../../APropos/bin/APropos.app/Contents/MacOS/APropos; then
    ../../APropos/bin/APropos.app/Contents/MacOS/APropos BrowserOnly ../../Doc/Start_MedinTux/start_medintux.html
fi
if test -f ../../APropos/bin/APropos; then
    ../../APropos/bin/APropos BrowserOnly ../../Doc/Start_MedinTux/start_medintux.html
fi
