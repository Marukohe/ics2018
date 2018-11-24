#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //char *buff = (char *)buf;
  for(int i=0;i<len;i++)
	_putc(((char *)buf)[i]);
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

/* my events_read*/
size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  if(key!=_KEY_NONE){
	if((key&0x8000)==0)  //down
	{
		//assert(0);
		//sprintf(buf,"ku %s\n",keyname[key]);
		snprintf(buf,len,"ku %s\n",keyname[key]);
	}
	else{    //code
		//sprintf(buf,"kd %s\n",keyname[key&0x7fff]);
		//assert(0);
		snprintf(buf,len,"kd %s\n",keyname[key&0x7fff]);
	}
  }
  else{
	  uint32_t tim = uptime();
	  //sprintf(buf,"t %d\n",tim);
	  snprintf(buf,len,"t %d\n",tim);
  }

  //return len-1;
  return strlen(buf);
}
/*
size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  bool down = false;
  if(key & 0x8000){
    key ^= 0x8000;
    down = true;
  } 
  if(key != _KEY_NONE){
    sprintf(buf, "%s %s\n", down?"kd":"ku", keyname[key]);
  } 
  else{
    sprintf(buf, "t %d\n", uptime());
  } 
  if(strlen(buf) > len){
    printf("\33[1;31mERROR: In events_read: buf is too long.\33[0m\n");
    assert(0);
  } 
  ((char*)buf)[len] = '\0';
  return strlen(buf);
  //return 0;
}
*/

static char dispinfo[128] __attribute__((used));

/*
myread
*/
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
	memcpy(buf,dispinfo+offset,len);
    //Log("buf:%soffset:%d",(char *)buf,offset);
  return strlen(buf);
}

/*
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  for(int i = 0;i < len;i++){ 
    ((char*)buf)[i] = dispinfo[offset+i];
  }
  ((char*)buf)[len] = '\0';
  //return len;
  //printf("dispinfo_read: len:%d\tstrlen:%d\n", len, strlen(buf));
  return strlen(buf);
}
*/
//int W = screen_width();
//int H = screen_height();

/*
my fbwrite
*/
/*maybe it always draw a rectangle*/
/*
size_t fb_write(const void *buf, size_t offset, size_t len) {
  //extern int screen_w,screen_h;
  //Log("screen:%d %d",screen_width(),screen_height());
  int W = screen_width();
  //int H = screen_height();
  offset/=4;
  int y = offset/W;
  int x = offset-W*y;
  len/=4;
  int lenA = len>W-x?W-x:len;
  int lenB = len-lenA>0?((len-lenA)/W>0?(len-lenA)/W*W:0):0;
  int lenC = len-lenA-lenB;
  draw_rect((uint32_t *)buf,x,y,lenA,1);
  if(lenB)
	  draw_rect((uint32_t *)buf+lenA,0,y+1,W,lenB/W);
  if(lenC)
	  draw_rect((uint32_t *)buf+lenA+lenB,0,y+1+lenB/W,lenC,1);
  return 0;
}
*/

size_t fb_write(const void *buf, size_t offset, size_t len) {
  
  int row,col;
  offset /= 4;
  col = offset % screen_width();
  row = offset / screen_width();
  //draw_rect((uint32_t*)buf, col, row, len/4, 1);
  draw_rect((uint32_t *)buf,col,row,screen_width(),(len/4)/screen_width());
  return len;
  //return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  //strcpy(dispinfo,"WIDTH:400\nHEIGHT:300");
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
  //assert(0);
  //Log("dispinfo:%s",dispinfo);
}
