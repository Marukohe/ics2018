#include "klib.h"

//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__)||defined(__NATIVE__USE__KLIB__)

size_t strlen(const char *s) {
	size_t i = 0;
	while(s[i]!='\0')
		i++;
	return i;
}

char *strcpy(char* dst,const char* src) {
	size_t i;
	//size_t sizesrc=strlen(src);
	for( i=0;src[i]!='\0';i++)
	   dst[i]=src[i];
	dst[i]='\0';
	return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
	size_t i;
	for(i = 0; i < n && src[i] != '\0'; i++)
		dst[i]=src[i];
	for( ; i<n; i++)
		dst[i]='\0';
	return dst;
}

char* strcat(char* dst, const char* src) {
	size_t dest_len = strlen(dst);
	size_t i;

	for(i=0;src[i]!='\0';i++)
		dst[dest_len+i]=src[i];
	dst[dest_len+i]='\0';
	return dst;
}

/*int strcmp(const char* s1, const char* s2) {
	size_t s1_len = strlen(s1); 
	size_t s2_len = strlen(s2);
	size_t cmp_len;
	if(s1_len<s2_len)
		cmp_len=s1_len;
	else
		cmp_len=s2_len;
	size_t i;
	int value=0;
	for(i=0;i<cmp_len;i++)
	{
		if(s1[i]-'\0'==s2[i]-'\0')
			continue;
		else
		{
			if(s1[i]-'\0'<s2[i]-'\0')
			{
				value=-1;
				return value;
			}
			else return 1;
		}
	}
	return value;
}
*/
int strcmp(const char* s1,const char* s2)
{
	int value=0;
	if(!s2||!s2) return -2;
	while(!(value = *(unsigned char*)s1-*(unsigned char*)s2)&&*s2){
		s1++;
		s2++;
	}
	if(value < 0)
		return -1;
	else if(value > 0)
		return 0;
	return value;
}
int strncmp(const char* s1, const char* s2, size_t n) {
	size_t i;
	int value=0;
	for(i=0;i<n;i++)
	{
		if(s1[i]-'\0'==s2[i]-'\0')
			continue;
		else
		{
			if(s1[i]-'\0'<s2[i]-'\0')
			{
				value=-1;
				return value;
			}
			else return 1;
		} 
	}
	return value;
}

void* memset(void* v,int c,size_t n) {
	const unsigned char ch = c;
	unsigned char *ret;
	while(n--)
	{
		ret=v;
		*ret++=ch;
	}
	ret = v;
	ret++;
	ret='\0';
	return v;
}

void* memcpy(void* out, const void* in, size_t n) {
	if(out==NULL||in==NULL||n<=0)
	   return NULL;
	unsigned char* pout = (unsigned char*)out;
	const unsigned char* pin = (const unsigned char*)in; 
	int falg1 = (pout>=pin && pout<pin+n);
	int falg2 = (pin>=pout && pin<pout+n);
	if(falg1 || falg2)
		return NULL;
	while(n--)
	{
		*pout=*pin;
		pout++;
		pin++;
	}
	return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
	if(s1==NULL||s2==NULL||n<0)
	   return 0;
	const unsigned char* dst = (const unsigned char*)s1;
	const unsigned char* src = (const unsigned char*)s2;
	while(n--){
		while(*dst==*src)
		{
			if(*dst=='\0')
				return 0;
			dst++;
			src++;
		}
		//if(n<=0)
		//	return 0;
	}
	if(n==-1)
		return 0;
	if(*dst>*src)
		return 1;
	else if(*dst<*src)
		return -1;
	return 0;
}

#endif
