@echo off
setlocal

set "QTDIR=D:\Qt\6.10.2\msvc2022_64"
set "VC_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community"
set "PROJECT_DIR=c:\Users\spong\Documents\trae_projects\world-cup"
set "BUILD_DIR=%PROJECT_DIR%\build_msvc"

echo 正在检查环境...

if not exist "%QTDIR%\bin\qmake.exe" (
    echo 错误: 找不到 Qt qmake
    pause
    exit /b 1
)

if not exist "%VC_DIR%\VC\Auxiliary\Build\vcvars64.bat" (
    echo 错误: 找不到 Visual Studio
    pause
    exit /b 1
)

echo 设置 Visual Studio 环境...
call "%VC_DIR%\VC\Auxiliary\Build\vcvars64.bat"

echo 设置 Qt 环境...
set PATH=%QTDIR%\bin;%PATH%

echo 创建构建目录...
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo 运行 qmake...
cd /d "%PROJECT_DIR%"
"%QTDIR%\bin\qmake.exe" PenaltyShootout.pro -o "%BUILD_DIR%\Makefile"

echo 运行 nmake...
cd /d "%BUILD_DIR%"
nmake

echo 复制国旗资源到输出目录...
if not exist "%BUILD_DIR%\debug\flags" mkdir "%BUILD_DIR%\debug\flags"
xcopy "%PROJECT_DIR%\flags\*.png" "%BUILD_DIR%\debug\flags\" /Y

echo 复制 Qt DLL...
xcopy "%QTDIR%\bin\Qt6Core.dll" "%BUILD_DIR%\debug\" /Y
xcopy "%QTDIR%\bin\Qt6Gui.dll" "%BUILD_DIR%\debug\" /Y
xcopy "%QTDIR%\bin\Qt6Widgets.dll" "%BUILD_DIR%\debug\" /Y
xcopy "%QTDIR%\plugins\platforms\qwindows.dll" "%BUILD_DIR%\debug\platforms\" /Y /E

echo 编译完成！
echo 可执行文件位置: %BUILD_DIR%\debug\PenaltyShootout.exe
pause