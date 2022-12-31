#include <stdio.h>
static int i=0x12345678;
int main(){
	// unsigned int i=0x12345678;
	unsigned char *addr;
	
	addr = (char *)&i;
	printf("ADDR = %p\n", addr);
	printf("%x %x %x %x\n", *addr, *addr++, *addr++, *addr++);
	printf("%x %x %x %x\n", i, i++, i++, i++);
	return 0;
}