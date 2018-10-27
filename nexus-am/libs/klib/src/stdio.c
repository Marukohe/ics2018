#include "klib.h"
#include <stdarg.h>

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__)||defined(__NATIVE_USE_KLIB__)

#define is_digit(c) ((c) >= '0' && (c) <='9')

static int skip_atoi(const char **s)
{
	int i=0;
	while(is_digit(**s)){
		i = i + *((*s)++)-'0';
	}
	return i;
}

/*int printf(const char *fmt, ...) {
  return 0;
}*/

void num_to_string(long num,int base,int flags,int width,int precision, char **s){
	char tmp[32];
	int i=0;
	const char *digits="0123456789ABCDEF";

	if(base<2 || base>16) return;

	if(num==0) tmp[i++]='0';
	while(num!=0)
	{
		tmp[i++] = digits[num%base];
		num=num/base;
	}
	width-=i;
	while(i-->0)
		*((*s)++) = tmp[i];
	while(width-->0)
		*((*s)++) = ' ';
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int len;
	int i;
	int width;
	char c;
	char *start=out;
	char* s;
	while((c=*fmt++)!=0)
	{
		if(c!='%'){ *out++=c; continue;}
		if(*fmt == '%'){ *out++ = '%'; fmt++; continue;}
        width=-1;
		if(is_digit(*fmt))
		   	width = skip_atoi((const char**)(&fmt));
		else if(*fmt == '*'){
			width = va_arg(ap,int);
			fmt++;
		}

		switch(*fmt){
			case 's':
				s=va_arg(ap,char *);
				len=strlen(s);
				for( i=0;i<len;i++)
					*out++=*s++;
				while(len<width--)
					*out++=' ';
				break;
			case 'd':
				num_to_string(va_arg(ap,int),10,0,width,0,&out);
				break;
			default:
				return -1;
		}
	}

	return (int)(out-start);
}

int sprintf(char *out, const char *fmt, ...) {
	va_list args;
	int res;
	va_start(args,fmt);
	res = vsprintf(out,fmt,args);
	va_end(args);
	return res;
}

int printf(const char *fmt, ...) {
	va_list args;
	int res;
	va_start(args,fmt);
	char *s;
	res=vsprintf(s,fmt,args);
	for(; *s ;s++){
		_puts(*s);
	}
	return res;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
