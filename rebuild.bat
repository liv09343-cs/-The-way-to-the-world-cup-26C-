@echo off

cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

call "D:\Qt\6.10.2\mingw_64\bin\qtenv2.bat"

"D:\Qt\6.10.2\mingw_64\bin\qmake.exe" PenaltyShootout.pro

"D:\Qt\6.10.2\mingw_64\bin\mingw32-make.exe"

echo 构建完成！