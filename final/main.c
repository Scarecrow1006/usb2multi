#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include "/usr/local/include/hidapi/hidapi.h"

#define MAX_STR 255
#define BUFFER_SIZE 50
#define LPCVID 0xc251
#define LPCPID 0x1301

unsigned char buf[65];

void cat(char fn[]);
int setmode(hid_device *handle);
void delay(int mili);
void send_usb(char c, hid_device *handle);
void reset_mode(hid_device *handle);

int main(int argc, char* argv[]){
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

	char outreport;
	int mode=0;
	char str[100];
	char reset[]="reset";
	char *endptr;
	char c;
	long x;
	int rstflag;
	
	while(1){
		rstflag=0;
		if (mode==0) mode = setmode(handle);
		else{
			while(1){
				printf(":");
				scanf("%s",str);
				x=strtol(str,&endptr,0);
				if(endptr==str){
					if(strcmp(str,reset)!=0){
						printf("retry\n");
						continue;
					}
					else{
						rstflag=1;
						break;
					}
				}
				else{
					c=(unsigned char) x;
					break;
				}
			}

			if(!rstflag) send_usb(c,handle);
			else {
				reset_mode(handle);
				mode=0;
			}
		}
	}

	hid_close(handle);
	return 0;
}

void cat(char fn[]){
	char buf[50];
	int file = open(fn, O_RDONLY);
	if (file == -1){
		fprintf(stdout, "Error: %s: file not found\n", fn);
		return;
	}
	int read_size;
	while ((read_size = read(file, buf, BUFFER_SIZE)) > 0)
		write(1, buf, read_size);
	close(file);
}

int setmode(hid_device *handle){
	int mode=0;
	cat("mode");
	while((mode==0) || (mode>=5)){
		printf(">");
		scanf("%d", &mode);
	}
	buf[0]=(unsigned char) mode;
	hid_write(handle, buf, 65);
	delay(10);
	switch(mode){
		case 1: 
			printf("Switching to GPIO...\n");
			break;
		case 2: 
			printf("Switching to SPI...\n");
			break;
		case 3: 
			printf("Switching to I2C...\n");
			break;
		case 4: 
			printf("Switching to PWM...\n");
			break;
	}
	cat("greet");
	return mode;
}

void delay(int mili) { 
	clock_t start_time = clock();
	while (clock() < start_time + mili*CLOCKS_PER_SEC/1000); 
}

void send_usb(char c, hid_device *handle){
	buf[0]=1;
	hid_write(handle, buf, 65);
	delay(10);
	buf[0]=(unsigned char) c;
	hid_write(handle, buf, 65);
	delay(10);
}

void reset_mode(hid_device *handle){
	buf[0]=0;
	hid_write(handle, buf, 65);
}