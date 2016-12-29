$path = pwd
cd ..\..\
mkdir -f build
mkdir -f build\x86
cd build\x86
cmake -G "Eclipse CDT4 - MinGW Makefiles" ..\..\Firmware\ -DTARGET_PLATFORM:STRING=x86
make -j5
& ".\build\x86\bin\unit_tests"
cd $path
