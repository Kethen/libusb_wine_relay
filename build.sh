set -xe
mkdir -p x86
mkdir -p x64

winegcc -m32 -shared -L/usr/lib/wine/i386-unix/ -L/usr/lib -lusb libusb0.c libusb0.spec -o x86/libusb0.dll.so
winegcc -m64 -shared -L/usr/lib64/wine/x86_64-unix -L/usr/lib64 -lusb libusb0.c libusb0.spec -o x64/libusb0.dll.so

