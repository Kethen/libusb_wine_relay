set -xe
mkdir -p aarch64

g++ -std=c++17 -fPIC -c libusb0.cpp -o aarch64/libusb0.o

wineg++ -shared -static-libgcc -static-libstdc++ aarch64/libusb0.o libusb0.spec -o aarch64/libusb0.dll.so -lusb

