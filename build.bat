@echo off

:: 进入项目目录
cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

:: 设置Qt环境变量
call "D:\Qt\6.10.2\mingw_64\bin\qtenv2.bat"

:: 生成Makefile
"D:\Qt\6.10.2\mingw_64\bin\qmake.exe" PenaltyShootout.pro

:: 构建项目
"D:\Qt\6.10.2\mingw_64\bin\mingw32-make.exe"

echo 构建完成！
pause