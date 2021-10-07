@echo off

:: 编译采用4线程
set BUILD_THREAD_COUNT=4
::工程名
set PROJECT_NAME=QTC.pro

::Qt相关路径
::set QMAKE_PATH=D:\Qt\Qt5.15.1\5.15.1\mingw81_64\bin
::set MINGW32_MAKE_PATH=D:\Qt\Qt5.15.1\Tools\mingw810_64\bin

set QMAKE_PATH=C:\Qt\Qt5.7.0\5.7\mingw53_32\bin
set MINGW32_MAKE_PATH=C:\Qt\Qt5.7.0\Tools\mingw530_32\bin

set cpath=%cd%\
::工程所在路径
set PROJECT_FULL_PATH=%cpath%\..\

::临时文件生成路径
set SHADOW_BUILD_PATH=%PROJECT_FULL_PATH%\tempFile\windows\Release
md %SHADOW_BUILD_PATH%


echo "QMAKE_PATH:" %QMAKE_PATH%
echo "PROJECT_FULL_PATH:" %PROJECT_FULL_PATH%
echo "MINGW32_MAKE_PATH:" %MINGW32_MAKE_PATH%

::生成MakeFile文件
%QMAKE_PATH%\qmake.exe %PROJECT_FULL_PATH%\%PROJECT_NAME% -o %SHADOW_BUILD_PATH%\Makefile  -r -spec win32-g++ CONFIG+=release CONFIG+=MinGW 

::编译
cd %SHADOW_BUILD_PATH%\

%MINGW32_MAKE_PATH%\mingw32-make.exe release -j%BUILD_THREAD_COUNT%

:: SVN 上传,需要本地安装的SVN支持
::svn add Bat\dsym\*
::svn add Release\*.dll
::svn commit -m "build program and commit "

pause