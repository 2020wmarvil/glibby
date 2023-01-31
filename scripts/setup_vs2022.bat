@echo off

cd ..
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..

echo.
echo Setup complete...
echo.

pause