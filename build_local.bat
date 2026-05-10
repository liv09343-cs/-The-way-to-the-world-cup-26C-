@echo on

cd /d "c:\Users\spong\Documents\trae_projects\world-cup"

echo 设置Visual Studio环境变量...
call "D:\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

echo 设置Qt环境变量...
set PATH=D:\Qt\6.10.2\msvc2022_64\bin;%PATH%

echo 检查qmake...
qmake --version

echo 创建构建目录...
if not exist release mkdir release
cd release

echo 生成Makefile...
qmake ..\PenaltyShootout.pro

echo 构建项目...
nmake -f Makefile.Release

echo 构建完成！
dir *.exe