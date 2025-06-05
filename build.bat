@echo off

cd /d "%~dp0"

mkdir build\win
cd build\win

cmake ../..

cmake --build . --config Release

pause