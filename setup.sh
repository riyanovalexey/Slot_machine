#!/bin/sh
echo "=== SlotMachine Build ==="

if ! which cmake > /dev/null 2>&1; then
    echo "Error: CMake not found."
    echo "Please install CMake from https://cmake.org/download/"
    exit 1
fi

if [ -f "external/SFML/include/SFML/Graphics.hpp" ]; then
    echo "Found local SFML."
    SFML_LOCAL_FOUND=1
else
    echo "Local SFML not found."
    printf "Install SFML via vcpkg? (y/n): "
    read INSTALL_CHOICE
    
    case "$INSTALL_CHOICE" in
        [Yy]*)
            if ! which git > /dev/null 2>&1; then
                echo "Error: Git not found."
                echo "Please install Git from https://git-scm.com/download/linux"
                exit 1
            fi

            if [ ! -d "vcpkg" ]; then
                echo "Installing vcpkg..."
                git clone https://github.com/Microsoft/vcpkg.git
                cd vcpkg
                ./bootstrap-vcpkg.sh -disableMetrics
                cd ..
            fi

            echo "Installing SFML..."
            ./vcpkg/vcpkg install
            if [ $? -ne 0 ]; then
                echo "Failed to install SFML."
                exit 1
            fi
            ;;
        *)
            echo "Please download SFML from https://www.sfml-dev.org/download.php"
            echo "Extract bin, lib, include folders to external/SFML folder and run this script again."
            exit 1
            ;;
    esac
fi

if [ ! -d "build" ]; then
    mkdir build
fi
cd build

echo "Configuring project..."
if [ -n "$SFML_LOCAL_FOUND" ]; then
    cmake .. -DUSE_LOCAL_SFML=ON
else
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DUSE_LOCAL_SFML=OFF
fi

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    cd ..
    exit 1
fi

echo "Building project..."
cmake --build . --config Release

if [ $? -ne 0 ]; then
    echo "Build failed!"
    cd ..
    exit 1
fi

echo "Build successful."
echo "Running application..."
cd bin/
./SlotMachine

cd ../../..
exit 0 