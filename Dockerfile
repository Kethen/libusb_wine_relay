FROM ubuntu:24.04
RUN export DEBIAN_FRONTEND=noninteractive; if [ -z "$(uname -a | grep aarch)" ]; then dpkg --add-architecture i386; fi
RUN export DEBIAN_FRONTEND=noninteractive; apt update; apt install -y libwine-dev libusb-dev libusb-0.1-4
RUN export DEBIAN_FRONTEND=noninteractive; apt update; if [ -z "$(uname -a | grep aarch)" ]; then apt install -y libusb-0.1-4:i386 gcc-multilib g++-multilib libwine-dev:i386 gcc g++; ln -s /usr/lib/i386-linux-gnu/libusb-0.1.so.4 /usr/lib/i386-linux-gnu/libusb.so; else apt install -y clang; rm /usr/bin/gcc /usr/bin/g++; ln -s /usr/bin/clang /usr/bin/gcc; ln -s /usr/bin/clang /usr/bin/g++ ;fi
