FROM ubuntu:24.04
RUN export DEBIAN_FRONTEND=noninteractive; if [ -z "$(uname -a | grep aarch)" ]; then dpkg --add-architecture i386; fi
RUN export DEBIAN_FRONTEND=noninteractive; apt update; apt install -y libwine-dev libusb-dev libusb-0.1-4 gcc g++
RUN export DEBIAN_FRONTEND=noninteractive; if [ -z "$(uname -a | grep aarch)" ]; then apt update; apt install -y libusb-0.1-4:i386 gcc-multilib g++-multilib libwine-dev:i386; ln -s /usr/lib/i386-linux-gnu/libusb-0.1.so.4 /usr/lib/i386-linux-gnu/libusb.so; fi

