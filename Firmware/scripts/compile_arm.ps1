$path = pwd
cd ..\..\
mkdir -f build
mkdir -f build\arm
cd build\arm
cmake -G "Eclipse CDT4 - MinGW Makefiles" ..\..\Firmware\ -DTARGET_PLATFORM:STRING=arm
make -j5
cd $path