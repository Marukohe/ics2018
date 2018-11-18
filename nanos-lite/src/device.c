#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char *buff = (char *)buf;
  for(int i=0;i<len;i++)
	_putc(buff[i]);
  return 0;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  if(key!=_KEY_NONE){
	if((key&0x8000)==0)  //down
		sprintf(buf,"ku %s\n",keyname[key]);
	else    //code
		sprintf(buf,"kd %s\n",keyname[key&0x7fff]);
  }
  else{
	  uint32_t tim = uptime();
	  sprintf(buf,"t %d\n",tim);
  }

  return len-1;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	strncpy(buf,dispinfo+offset,len);
    Log("buf:%soffset:%d",(char *)buf,offset);
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  //extern int screen_w,screen_h;
  Log("screen:%d %d",screen_width(),screen_height());
  offset/=4;
  int y = offset/screen_width();
  int x = offset-screen_width()*y;
  len/=4;
  int lenA = len>screen_width()-x?screen_width()-x:len;
  int lenB = len-lenA>0?((len-lenA)/screen_width()>0?(len-lenA)/screen_width()*screen_width():0):0;
  int lenC = len-lenA-lenB;
  draw_rect((uint32_t *)buf,x,y,lenA,1);
  if(lenB)
	  draw_rect((uint32_t *)buf+lenA,0,y+1,screen_width(),lenB/screen_width());
  if(lenC)
	  draw_rect((uint32_t *)buf+lenA+lenB,0,y+1+lenB/screen_width(),lenC,1);
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  strcpy(dispinfo,"WIDTH:640\nHEIGHT:480");
  Log("dispinfo:%s",dispinfo);
}
