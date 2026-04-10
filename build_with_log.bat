@echo off

:: 进入项目目录
cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

:: 设置Visual Studio环境变量
call "D:\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: 设置Qt环境变量
call "D:\Qt\6.10.2\msvc2022_64\bin\qtenv2.bat"

:: 生成Makefile
"D:\Qt\6.10.2\msvc2022_64\bin\qmake.exe" "c:\Users\spong\Documents\trae_projects\world-cup\PenaltyShootout.pro"

:: 构建项目
nmake > build.log 2>&1

echo 构建完成！
type build.log
pause