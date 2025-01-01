#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <usb.h>
#else
#include <lusb0_usb.h>
#endif

int main(){
	usb_init();

	for(int i = 0;i < 3; i++){
		usb_find_busses();
		usb_find_devices();
		struct usb_bus *bus;
		struct usb_device *dev;
		bus = usb_get_busses();
		while(bus != NULL){
			printf("%s: bus %s\n", __func__, bus->dirname);
			dev = bus->devices;
			while(dev != NULL){
				printf("%s: %04x:%04x\n", __func__, dev->descriptor.idVendor, dev->descriptor.idProduct);
				dev = dev->next;
			}
			bus = bus->next;
		}
	}
	return 0;
}
