echo "================= compilation de med_stat ==========================="
Z:
cd Z:/Documents/medintux/Programmes/set_bases/bin/
set QMAKE=C:/Qt3/3.3.4\bin\qmake.exe
set QMAKESPEC=win32-msvc
set MODULE=med_stat
cd ../../%MODULE%/src
echo "================= se placer dans %MODULE%/src ==========================="
erase %MODULE%.dsw
erase %MODULE%.dsp
if exist %MODULE%.pro %QMAKE% -t vcapp -o %MODULE%.dsp %MODULE%.pro
if exist src.pro %QMAKE% -t vcapp -o %MODULE%.dsp src.pro
cd ../../mise_a_jour/bin
mise_a_jour.exe MSDEV  ../../%MODULE%/src/%MODULE%.dsp %MODULE%.exe
cd ../../%MODULE%/src
msdev %MODULE%.dsp /MAKE /CLEAN
msdev %MODULE%.dsp /MAKE ALL
