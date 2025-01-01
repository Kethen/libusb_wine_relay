x86_64-w64-mingw32-gcc -L ./ -lusb0_x64 -I ./ libusb_test.c -o libusb_test_x64.exe
i686-w64-mingw32-gcc -L ./ -lusb0_x86 -I ./ libusb_test.c -o libusb_test_x86.exe
gcc -lusb libusb_test.c -o libusb_test.out
