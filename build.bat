@echo off

set TOOLS=%~dp0\Development\Tools

echo [BUILD.BAT] GENERATE CMAKE FILES
if exist build (
    cd build
    call "../Development/Tools/cmake-windows/bin/cmake.exe" ..
    call "../Development/Tools/cmake-windows/bin/cmake.exe" --build .

    goto exit
)

mkdir build
cd build

@echo ==============================
@echo 1. Visual Studio 2022
@echo 2. Visual Studio 2019
@echo 3. Ninja
@echo ==============================

:TOP
set /p choice=choose:
goto %choice% & goto exit

:1
call "../Development/Tools/cmake-windows/bin/cmake.exe" ^
    -G "Visual Studio 17 2022" ^
    -DCMAKE_BUILD_TYPE=Debug .. ^
    -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET="x64-windows" 

goto exit

:2
call "../Development/Tools/cmake-windows/bin/cmake.exe" ^
    -G "Visual Studio 16 2019" ^
    -DCMAKE_BUILD_TYPE=Debug .. ^
    -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET="x64-windows" 

goto exit

:3
call "../Development/Tools/cmake-windows/bin/cmake.exe" -G "Ninja" ^
    -DCMAKE_BUILD_TYPE=Debug .. ^
    -DCMAKE_TOOLCHAIN_FILE="../vcpkg/scripts/buildsystems/vcpkg.cmake" ^
    -DVCPKG_TARGET_TRIPLET="x64-windows" 

goto exit

:exit
cd ..
