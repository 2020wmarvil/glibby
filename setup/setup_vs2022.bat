@echo off

cd ..
mkdir ProjectFiles
cd ProjectFiles
cmake -G "Visual Studio 17 2022" ..

echo.
echo Setup complete...
echo.

pause