#include "trap.h"

char buf[128];

int main() {
	sprintf(buf, "%s", "Hello world!\n");
	nemu_assert(strcmp(buf, "Hello world!\n") == 0);

	//nemu_assert(strcmp("1 + 1 = 2\n","1 + 1 = 2\n")==0);
	sprintf(buf, "%d + %d = %d\n", 1, 1, 2);
	nemu_assert(strcmp(buf, "1 + 1 = 2\n") == 0);

	sprintf(buf, "%d + %d = %d\n", 2, 10, 12);
	//printf("%s",buf);
	nemu_assert(strcmp(buf, "2 + 10 = 12\n") == 0);

	char buff[256];
	snprintf(buff,5,"%d + %d = %d\n",2,10,12);
	printf("%s\n",buff);
	nemu_assert(strcmp(buff,"2 + 1")==0);
	//nemu_assert(strcmp("2d + 10d = 12d","2 + 10 = 12")==0);
	//int a=10;
	//char *s="hello,world";
	//printf("%03d\n",a);
	//printf("%d  %s\n",a,s);
	return 0;
}
