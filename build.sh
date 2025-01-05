set -xe
mkdir -p x86
mkdir -p x64

g++ -m32 -std=c++17 -fPIC -c libusb0.cpp -o x86/libusb0.o
g++ -m64 -std=c++17 -fPIC -c libusb0.cpp -o x64/libusb0.o

wineg++ -m32 -L/usr/lib/i386-linux-gnu -L/usr/lib/i386-linux-gnu/wine/i386-unix -shared -static-libgcc -static-libstdc++ x86/libusb0.o libusb0.spec -o x86/libusb0.dll.so -lusb
wineg++ -m64 -shared -static-libgcc -static-libstdc++ x64/libusb0.o libusb0.spec -o x64/libusb0.dll.so -lusb

