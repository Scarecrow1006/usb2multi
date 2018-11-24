#include <stdlib.h>
#include <stdio.h>
#include <errno.h>	
#include <string.h>
#include <time.h>

void delay(int mili) { 
	clock_t start_time = clock();
	while (clock() < start_time + mili*CLOCKS_PER_SEC/1000) ; 
}

int main(){
	char str[]="reset";
	while(1){
		printf("%s\n", str);
		delay(1000);	
	}
	
	return 0;
}