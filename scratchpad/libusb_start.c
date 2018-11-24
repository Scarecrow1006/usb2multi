#include "usb2multi.h"

int libusb_start(session *s){
	int x1=libusb_init(&(s->ctx));
	if(x1==0) printf("success init\n");
	else {
		printf("fail init\n");
		return 1;
	}

	ssize_t count=libusb_get_device_list(s->ctx,&(s->list));
	ssize_t i=0;

	uint16_t vid;
	uint16_t pid;

	s->found=NULL;
	for (i=0;i<count;i++){
		printf("device %d\n", (int)i);
		libusb_get_device_descriptor((s->list[i]), &(s->desc));
		vid=(s->desc).idVendor;
		pid=(s->desc).idProduct;
		if (vid==LPC2148_VID && pid==LPC2148_PID){
			s->found=(s->list[i]);
		}
		printf("pid: %x, vid: %x\n", pid,vid);
	}
	printf("no of usb devices detected = %d\n", (int)count);
	if(s->found==NULL){
		printf("specified device not found\n");
		return 2;
	}

	int x3=libusb_open(s->found,&(s->h));
	if(x3==0) printf("open success\n");
	else {
		printf("open fail\n");
		return 3;
	}

	libusb_free_device_list(s->list,1);

	libusb_get_active_config_descriptor(s->found, &(s->config));
	s->alts=(s->config->interface[0]).altsetting[0];
	return 0;
}

void libusb_end(session *s){
	libusb_free_config_descriptor(s->config);
	libusb_close(s->h);
	libusb_exit(s->ctx);
}