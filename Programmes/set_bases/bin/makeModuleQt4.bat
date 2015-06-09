echo "================= compilation de personnes ==========================="
Z:
cd Z:/Documents/medintux/Programmes/set_bases/bin/
set MODULE=personnes
set QTDIR=C:/Qt4W/qt/
set QMAKE=%QTDIR%/bin/qmake.exe
set QMAKESPEC=%QTDIR%/mkspecs/win32-g++
set PATH=%PATH%;%QTDIR%/bin;%QTDIR%/../mingw/bin
cd ../../%MODULE%/src
if exist %MODULE%.pro %QMAKE% -spec %QMAKESPEC% -win32 CONFIG+=release -o Makefile %MODULE%.pro
if exist src.pro %QMAKE% -spec %QMAKESPEC% -win32 CONFIG+=release -o Makefile src.pro
%QTDIR%/../mingw/bin/mingw32-make.exe clean
%QTDIR%/../mingw/bin/mingw32-make.exe
