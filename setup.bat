@echo off
setlocal enabledelayedexpansion
echo === SlotMachine Build ===

where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake not found.
    echo Please install CMake from https://cmake.org/download/
    pause > nul
    exit /b 1
)

if exist "external\SFML\include\SFML\Graphics.hpp" (
    echo Found local SFML.
    set SFML_LOCAL_FOUND=1
) else (
    echo Local SFML not found.
    set /p INSTALL_CHOICE="Install SFML via vcpkg? (y/n): "
    
    if /i "!INSTALL_CHOICE!"=="y" (
        where git >nul 2>nul
        if %ERRORLEVEL% NEQ 0 (
            echo Error: Git not found.
            echo Please install Git from https://git-scm.com/download/win
            pause > nul
            exit /b 1
        )

        if not exist vcpkg (
            echo Installing vcpkg...
            git clone https://github.com/Microsoft/vcpkg.git
            cd vcpkg
            call bootstrap-vcpkg.bat -disableMetrics
            cd ..
        )

        echo Installing SFML...
        vcpkg install
        if %ERRORLEVEL% NEQ 0 (
            echo Failed to install SFML.
            pause > nul
            exit /b 1
        )
    ) else (
        echo Please download SFML from https://www.sfml-dev.org/download.php
        echo Extract bin, lib, include folders to external/SFML folder and run this script again.
        pause > nul
        exit /b 1
    )
)

if not exist "build" mkdir build
cd build

echo Configuring project...
if defined SFML_LOCAL_FOUND (
    cmake .. -DUSE_LOCAL_SFML=ON
) else (
    cmake .. -DCMAKE_TOOLCHAIN_FILE=..\vcpkg\scripts\buildsystems\vcpkg.cmake -DUSE_LOCAL_SFML=OFF
)

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    cd ..
    pause > nul
    exit /b 1
)

echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    cd ..
    pause > nul
    exit /b 1
)

echo Build successful.
echo Running application...
cd bin\Release
SlotMachine.exe

cd ..\..\..\
exit /b 0 