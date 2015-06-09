@echo off
rem Make Mingw
set MAKE=mingw32-make 

set FileErr=.\Err.Mt
if '%1' =='' goto finfin

cd ..\..\%1\src

echo  ****Compilation du module %1******
echo      - Genere Makefile
   qmake src.pro > NUL 2>>%FileErr%
echo      - Make clean
   %MAKE% clean > NUL 2>>%FileErr%
echo      - Compilation du module %1
   %MAKE% > NUL 2>>%FileErr%

if not %ERRORLEVEL%==0 goto :FinErr

   cd ..\bin
echo      - Strip de l'executable %1.exe
   strip -s %1.exe  
REM > NUL 2>>%FileErr%

cd ..\..\set_bases\bin
goto finfin

:FinErr

echo Erreur de compilation, Fin du Programme
type %FileErr%
pause
SET ERRORCOMPILE=1

:finfin
