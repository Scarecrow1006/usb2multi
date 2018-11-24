#include <stdio.h>
#include <stdlib.h>

#include "/usr/local/include/hidapi/hidapi.h"

#define MAX_STR 255
#define LPCVID 0xc251
#define LPCPID 0x1301

int main(int argc, char* argv[]){
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;

	struct hid_device_info *devs, *cur_dev;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
			cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	handle = hid_open(LPCVID, LPCPID, NULL);
	if (handle==NULL){
		printf("Device could not be opened.\n");
		return 1;
	}

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);

	char outreport;
	int x;
	while(1){
		printf(":");
		scanf("%d", &x);
		buf[0]=(unsigned char)x;
		hid_write(handle,buf,65);
	}

	return 0;
}