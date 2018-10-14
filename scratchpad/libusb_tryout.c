#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LPC2148_VID 0x10c4
#define LPC2148_PID 0xea60

int main(int argc, char const *argv[])
{
	libusb_context *ctx;
	int x=libusb_init(&ctx);

	if(x==0) printf("success init\n");
	else printf("fail init\n");
	
	libusb_device **list;
	libusb_device *found=NULL;
	ssize_t count=libusb_get_device_list(ctx,&list);
	ssize_t i=0;

	struct libusb_device_descriptor desc;
	uint16_t vid;
	uint16_t pid;

	for (i=0;i<count;i++){
		printf("device %d\n", (int)i);
		libusb_get_device_descriptor(list[i], &desc);
		vid=desc.idVendor;
		pid=desc.idProduct;
		if (vid==LPC2148_VID && pid==LPC2148_PID)
		{
			found=list[i];
		}
		printf("pid: %x, vid: %x\n", pid,vid);
	}

	libusb_device_handle *h;
	x=libusb_open(found,&h);
	if(x==0) printf("open success\n");
	else printf("open fail\n");

	libusb_free_device_list(list,1);

	printf("no of usb devices detected = %d\n", (int)count);

	/*vid=0x10c4;
	pid=0xea60;

	libusb_device_handle *h=libusb_open_device_with_vid_pid(ctx,vid,pid);
	if(h==NULL) printf("open fail\n");
	else printf("open success\n");*/

	libusb_close(h);

	libusb_exit(ctx);
	return 0;
}