#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>

#define W 400
#define H 300
static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = W;
      info->height = H;
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
       //_VideoInfoReg info;
	   //dev->read(_DEVREG_VIDEO_INFO,&info,sizeof(info));

      _FBCtlReg *ctl = (_FBCtlReg *)buf;
	  int x=ctl->x;
	  int y=ctl->y;
	  int w=ctl->w;
	  int h=ctl->h;
	  uint32_t *pixels=ctl->pixels;
	  //int cnt=0;
	  for(int i=0;i<=w;i++){
		  for(int j=0;j<=h;j++){
			  fb[(y+j)*W+x+i-1]=pixels[i*w+j-1];
		  }
	  }
	 // int i;
	 // int size = screen_width() * screen_height();
	 // for(i=0;i<size;i++) fb[i]=i;

      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
}
