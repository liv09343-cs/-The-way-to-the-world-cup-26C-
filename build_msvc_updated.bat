@echo off
setlocal

cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

if not exist "build_msvc" mkdir build_msvc

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if %errorlevel% neq 0 (
    echo 无法找到 Visual Studio 环境变量
    pause
    exit /b 1
)

call "D:\Qt\6.10.2\msvc2022_64\bin\qtenv2.bat"
if %errorlevel% neq 0 (
    echo 无法设置 Qt 环境变量
    pause
    exit /b 1
)

echo 运行 qmake...
"D:\Qt\6.10.2\msvc2022_64\bin\qmake.exe" PenaltyShootout.pro -o build_msvc/Makefile
if %errorlevel% neq 0 (
    echo qmake 失败
    pause
    exit /b 1
)

cd build_msvc

echo 运行 nmake...
nmake
if %errorlevel% neq 0 (
    echo nmake 失败
    pause
    exit /b 1
)

echo 构建完成！
pause