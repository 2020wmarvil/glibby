@echo off

cd ..
cmake-format.exe -i CMakeLists.txt src/CMakeLists.txt deps/CMakeLists.txt examples/CMakeLists.txt tests/CMakeLists.txt docs/CMakeLists.txt 

pause