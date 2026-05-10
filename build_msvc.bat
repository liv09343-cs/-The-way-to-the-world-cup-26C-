@echo off

cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

call "D:\Qt\6.10.2\msvc2022_64\bin\qtenv2.bat"

"D:\Qt\6.10.2\msvc2022_64\bin\qmake.exe" PenaltyShootout.pro -o build_msvc/Makefile

cd build_msvc

nmake

echo 构建完成！
pause