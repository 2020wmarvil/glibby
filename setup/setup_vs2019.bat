@echo off

cd ..
mkdir ProjectFiles
cd ProjectFiles
cmake -G "Visual Studio 16 2019" ..

echo.
echo Setup complete...
echo.

pause