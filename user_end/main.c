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
void send_usb(char cmd, char data, hid_device *handle);
void reset_mode(hid_device *handle);
unsigned char read_usb(hid_device *handle);

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

	hid_set_nonblocking(handle, 0);

	char outreport;
	int mode=0;
	char str[100];
	char reset[]="reset";
	char exit[]="exit";
	char read[]="read";
	char *endptr;
	unsigned char c;
	long x;
	int rstflag,exitflag,rdflag;
	
	while(1){
		rstflag=0;
		exitflag=0;
		rdflag=0;
		//if (mode==5) rdflag=1;
		if (mode==0) 
			mode = setmode(handle);
		else{
			while(1){
				printf(":");
				scanf("%s",str);
				x=strtol(str,&endptr,0);
				if(endptr==str){
					if(strcmp(str,reset)==0){
						rstflag=1;
						break;
					}
					else if (strcmp(str,exit)==0){
						exitflag=1;
						break;
					}
					else if (strcmp(str,read)==0){
						rdflag=1;
						break;
					}
					else{
						printf("retry\n");
						continue;
					}
				}
				else{
					c=(unsigned char) x;
					break;
				}
			}

			if(exitflag) break;
			else if(rstflag){
				reset_mode(handle);
				mode=0;
			}
			else if(rdflag){
				switch(mode){
					case 1:
					case 2:
					case 4:
						printf("read invalid for current mode\n");
						break;
					case 3:
					case 5:
						c=read_usb(handle);
						printf("]%u\n", (unsigned int) c);
						break;
				}
			}
			else send_usb(1,c,handle);
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
	int slvaddr;
	cat("mode");
	while((mode==0) || (mode>=6)){
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
			printf("I2C slave address: ");
			scanf("%d", &slvaddr);
			send_usb(2,(unsigned char) slvaddr, handle);
			break;
		case 4: 
			printf("Switching to PWM...\n");
			break;
		case 5:
			printf("Switching to ADC read\n");
			printf("Enter 'read' to read digital value.(8-bit)\n");
			break;
	}
	cat("greet");
	return mode;
}

void delay(int mili) {
	clock_t start_time = clock();
	while (clock() < start_time + mili*CLOCKS_PER_SEC/1000); 
}

void send_usb(char cmd, char data, hid_device *handle){
	buf[0]=(unsigned char) cmd;
	hid_write(handle, buf, 65);
	delay(10);
	buf[0]=(unsigned char) data;
	hid_write(handle, buf, 65);
	delay(10);
}

void reset_mode(hid_device *handle){
	buf[0]=0;
	hid_write(handle, buf, 65);
}

unsigned char read_usb(hid_device *handle){
	unsigned char c=0;
	hid_read(handle, &c, 1);
	return c;
}
