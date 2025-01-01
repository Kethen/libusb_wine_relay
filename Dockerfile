FROM opensuse/leap:15.6
RUN zypper -n install wine-staging-devel wine-staging-devel-32bit gcc gcc-32bit gcc-c++ gcc-c++-32bit libusb-0_1-4 libusb-0_1-4-32bit libusb-compat-devel
RUN ln -s /usr/lib/libusb-0.1.so.4 /usr/lib/libusb.so

