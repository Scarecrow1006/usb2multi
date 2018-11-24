#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "usb2multi.h"

int main(int argc, char const *argv[])
{
	session s;
	int x=libusb_start(&s);
	if (x!=0){
		printf("'start' returned with exit code %d\n", x);
		//libusb_exit(s.ctx);
		return 1;
	}
	
	struct libusb_endpoint_descriptor ep[2];
	uint8_t epaddr[2];
	
	printf("num config: %d\n", (int) (s.desc.bNumConfigurations));
	printf("num interfaces: %d\n",(int) (s.config->bNumInterfaces));
	printf("num altsetting: %d\n",(int) ((s.config->interface[0]).num_altsetting));
	
	ep[0]=s.alts.endpoint[0];
	epaddr[0]=(ep[0]).bEndpointAddress;
	ep[1]=s.alts.endpoint[1];
	epaddr[1]=(ep[1]).bEndpointAddress;	
	printf("num endpoints: %d\n", (int) s.alts.bNumEndpoints);
	printf("ep_addr: %d, %d\n",(int) epaddr[0], (int) epaddr[1]);
	
	libusb_end(&s);
	return 0;
}