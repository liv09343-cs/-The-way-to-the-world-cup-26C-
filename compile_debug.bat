@echo off

call "D:\Qt\6.10.2\mingw_64\bin\qtenv2.bat"

cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

echo Running qmake...
"D:\Qt\6.10.2\mingw_64\bin\qmake.exe" "c:\Users\spong\Documents\trae_projects\world-cup\PenaltyShootout.pro"

echo Running make...
"D:\Qt\6.10.2\mingw_64\bin\mingw32-make.exe" 2>&1

echo Build completed!
pause