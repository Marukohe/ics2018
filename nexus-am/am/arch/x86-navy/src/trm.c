#include <am.h>
#include <stdio.h>
#include <stdlib.h>

extern int main();
void _trm_init() {
	int ret = main();
	_halt(ret);
}

void _putc(char ch) {
	putchar(ch);
}

void _halt(int code) {
	exit(code);
	while(1);
}

_Area _heap;
