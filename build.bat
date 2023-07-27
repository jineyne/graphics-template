@echo off

set TOOLS=%~dp0\Development\Tools

echo [BUILD.BAT] GENERATE CMAKE FILES
mkdir build
cd build

call "../Development/Tools/cmake-windows/bin/cmake.exe" -G "Ninja" ^
    -DCMAKE_BUILD_TYPE=Debug .. ^
    -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET="x64-windows" 

call ninja

cd ..
