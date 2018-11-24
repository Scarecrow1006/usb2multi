#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <ctype.h>

typedef struct usb2multi_session{
	libusb_context *ctx;
	libusb_device **list;
	libusb_device *found;
	struct libusb_device_descriptor desc;
	libusb_device_handle *h;
	struct libusb_config_descriptor *config;
	struct libusb_interface_descriptor alts;
}session;

#define LPC2148_VID 0xc251
#define LPC2148_PID 0x1301

int libusb_start(session*);
void libusb_end(session*);