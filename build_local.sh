set -xe
mkdir -p x86
mkdir -p x64

g++ -m32 -std=c++17 -fPIC -c libusb0.cpp -o x86/libusb0.o
#g++ -m64 -std=c++17 -fPIC -c libusb0.cpp -o x64/libusb0.o

wineg++ -m32 -shared -L/usr/lib/wine/i386-unix -L/usr/lib -lusb -static-libgcc -static-libstdc++ x86/libusb0.o libusb0.spec -o x86/libusb0.dll.so
#wineg++ -m64 -shared -L/usr/lib64/wine/x86_64-unix -L/usr/lib64 -static-libgcc -static-libstdc++ -lusb x64/libusb0.o libusb0.spec -o x64/libusb0.dll.so

