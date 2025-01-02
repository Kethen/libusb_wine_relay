#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <usb.h>

#include <map>

#define PRINT_CALLS 0
#if PRINT_CALLS
#define PRINT_CALL(name) {printf("calling " STR(name) "\n");}
#else
#define PRINT_CALL(name)
#endif

// dirty size differences
struct usb_bus_;
struct usb_device_ {
	struct usb_device_ *next, *prev;
	char filename[512];
	struct usb_bus_ *bus;
	struct usb_device_descriptor descriptor;
	struct usb_config_descriptor *config;
	void *dev;
	unsigned char devnum;
	unsigned char num_children;
	struct usb_device_ **children;
};

struct usb_bus_ {
	struct usb_bus_ *next, *prev;
	char dirname[512];
	struct usb_device_ *devices;
	unsigned long location;
	struct usb_device_ *root_dev;
};

// XXX what was the thread model of libusb0?
static struct usb_bus_ *busses = NULL;
static std::map<struct usb_device_ *, struct usb_device *> usb_device_map;

static void copy_device(struct usb_device_ *dst, struct usb_device *src, struct usb_bus_ *bus){
	memset(dst->filename, 0, sizeof(dst->filename));
	strncpy(dst->filename, src->filename, sizeof(dst->filename) - 1);
	dst->bus = bus;
	dst->descriptor = src->descriptor;
	dst->config = src->config;
	dst->dev = src->dev;
	dst->devnum = src->devnum;
	dst->num_children = src->num_children;
	if(src->num_children != 0){
		dst->children = (struct usb_device_ **)malloc(sizeof(struct usb_device_ *) * src->num_children);
		for(int i = 0; i < src->num_children; i++){
			struct usb_device_ *new_dev = (struct usb_device_ *)malloc(sizeof(struct usb_device_));
			new_dev->next = NULL;
			new_dev->prev = NULL;
			new_dev->bus = bus;
			usb_device_map[new_dev] = src->children[i];
			dst->children[i] = new_dev;
			copy_device(new_dev, src->children[i], bus);
		}
	}else{
		dst->children = NULL;
	}
}

static void cleanup_busses_devices(){
	for(auto itr = usb_device_map.begin(); itr != usb_device_map.end(); itr++){
		if(itr->first->children != NULL){
			free(itr->first->children);
		}
		free(itr->first);
	}
	while(busses != NULL){
		auto cur = busses;
		busses = busses->next;
		free(cur);
	}
	usb_device_map.clear();
}

static struct usb_device *find_device_ref(struct usb_device_ *mapped){
	auto itr = usb_device_map.find(mapped);
	if(itr != usb_device_map.end()){
		return itr->second;
	}

	return NULL;
}

extern "C" {
	#ifdef __x86_64__
	#define CALL_CONVENTION __attribute__ ((ms_abi))
	#else
	#define CALL_CONVENTION __attribute__ ((cdecl))
	#endif

	#define STR(s) #s
	#define PASSTHROUGH(name, ret_type, param, args) CALL_CONVENTION ret_type name##_ param { \
		PRINT_CALL(name) \
		return name args; \
	}

	PASSTHROUGH(usb_bulk_read, int, (usb_dev_handle *dev, int ep, char *bytes, int size, int timeout), (dev, ep, bytes, size, timeout))
	PASSTHROUGH(usb_bulk_write, int, (usb_dev_handle *dev, int ep, const char *bytes, int size, int timeout), (dev, ep, bytes, size, timeout))
	PASSTHROUGH(usb_claim_interface, int, (usb_dev_handle *dev, int interface), (dev, interface))
	PASSTHROUGH(usb_close, int, (usb_dev_handle *dev), (dev))
	PASSTHROUGH(usb_find_busses, int, (), ())
	PASSTHROUGH(usb_find_devices, int, (), ())
	// PASSTHROUGH(usb_get_busses, struct usb_bus *, (), ())
	PASSTHROUGH(usb_init, void, (), ())
	// PASSTHROUGH(usb_open, usb_dev_handle *, (struct usb_device *dev), (dev))
	PASSTHROUGH(usb_release_interface, int, (usb_dev_handle *dev, int interface), (dev, interface))
	PASSTHROUGH(usb_reset, int, (usb_dev_handle *dev), (dev))
	PASSTHROUGH(usb_set_configuration, int, (usb_dev_handle *dev, int configuration), (dev, configuration))

	CALL_CONVENTION struct usb_bus_ * usb_get_busses_(){
		cleanup_busses_devices();

		struct usb_bus *bus_first = usb_get_busses();
		struct usb_bus *bus = bus_first;
		struct usb_bus_ *prev = NULL;
		while(bus != NULL){
			struct usb_bus_ *cur = (struct usb_bus_ *)malloc(sizeof(struct usb_bus_));
			if(prev != NULL){
				prev->next = cur;
			}
			cur->prev = prev;
			cur->next = NULL;
			if(busses == NULL){
				busses = cur;
			}

			memset(cur->dirname, 0, sizeof(cur->dirname));
			strncpy(cur->dirname, bus->dirname, sizeof(cur->dirname) - 1);
			cur->devices = NULL;

			struct usb_device *device = bus->devices;
			struct usb_device_ *prev_dev = NULL;
			cur->devices = NULL;
			while(device != NULL){
				struct usb_device_ *cur_dev = (struct usb_device_ *)malloc(sizeof(struct usb_device_));
				if(prev_dev != NULL){
					prev_dev->next = cur_dev;
				}
				cur_dev->prev = prev_dev;
				cur_dev->next = NULL;
				if(cur->devices == NULL){
					cur->devices = cur_dev;
				}

				copy_device(cur_dev, device, cur);

				usb_device_map[cur_dev] = device;

				prev_dev = cur_dev;
				device = device->next;
			}
			cur->location = bus->location;
			if(bus->root_dev != NULL){
				cur->root_dev = (struct usb_device_ *)malloc(sizeof(struct usb_device_ *));
				cur->root_dev->next = NULL;
				cur->root_dev->prev = NULL;
				copy_device(cur->root_dev, bus->root_dev, cur);
				usb_device_map[cur->root_dev] = bus->root_dev;
			}else{
				cur->root_dev = NULL;
			}

			prev = cur;
			bus = bus->next;
		}

		return busses;
	}

	CALL_CONVENTION usb_dev_handle *usb_open_(struct usb_device_ *dev){
		struct usb_device *ref = find_device_ref(dev);
		if(ref == NULL){
			return NULL;
		}
		return usb_open(ref);
	}
}
