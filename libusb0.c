#include <usb.h>

#define PASSTHROUGH(name, ret_type, param, args) ret_type name##_ param {return name args;}

PASSTHROUGH(usb_bulk_read, int, (usb_dev_handle *dev, int ep, char *bytes, int size, int timeout), (dev, ep, bytes, size, timeout))
PASSTHROUGH(usb_bulk_write, int, (usb_dev_handle *dev, int ep, const char *bytes, int size, int timeout), (dev, ep, bytes, size, timeout))
PASSTHROUGH(usb_claim_interface, int, (usb_dev_handle *dev, int interface), (dev, interface))
PASSTHROUGH(usb_close, int, (usb_dev_handle *dev), (dev))
PASSTHROUGH(usb_find_busses, int, (), ())
PASSTHROUGH(usb_find_devices, int, (), ())
PASSTHROUGH(usb_get_busses, struct usb_bus *, (), ())
PASSTHROUGH(usb_init, void, (), ())
PASSTHROUGH(usb_open, usb_dev_handle *, (struct usb_device *dev), (dev))
PASSTHROUGH(usb_release_interface, int, (usb_dev_handle *dev, int interface), (dev, interface))
PASSTHROUGH(usb_reset, int, (usb_dev_handle *dev), (dev))
PASSTHROUGH(usb_set_configuration, int, (usb_dev_handle *dev, int configuration), (dev, configuration))
