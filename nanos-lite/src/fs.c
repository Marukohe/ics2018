#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
extern size_t serial_write(const void *buf,size_t offset,size_t len);

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB,FD_DISPINFO,FD_EVENTS};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t serial_write(const void *buf,size_t offset ,size_t len);
size_t fb_write(const void *buf,size_t offset,size_t len);
size_t dispinfo_read(void *buf,size_t offset,size_t len);
size_t events_read(void *buf,size_t offset,size_t len);
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"/proc/dispinfo", 128, 0, 0, dispinfo_read, invalid_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
	//file_table[FD_FB].size = 640*480*4;
	//Log("screen at fs:w%d h:%d",screen_width(),screen_height());
	file_table[FD_FB].size = screen_width()*screen_height()*4;
	file_table[FD_FB].open_offset = 0;
}

int fs_open(const char *pathname,int flags,int mode){
 	for(int i=0;i<NR_FILES;i++){
		if(strcmp(file_table[i].name,pathname)==0)
		{
			Log("%s fd:%d",pathname,i);
			return i;
 	 	}
 	} 
	Log("to see assert %s",pathname);
	assert(0);
	return 0;
}
extern size_t ramdisk_read(void *buf,size_t offset,size_t len);
extern size_t ramdisk_write(const void *buf,size_t offset,size_t len);
ssize_t fs_read(int fd,void *buf,size_t len){
	/*
	switch(fd){
		case FD_DISPINFO:
			Log("filesize:%d",file_table[FD_DISPINFO].size);
			if(file_table[FD_DISPINFO].open_offset+len==file_table[FD_DISPINFO].size)
				return 0;
			else if(file_table[FD_DISPINFO].open_offset+len>file_table[FD_DISPINFO].size)
				len = file_table[FD_DISPINFO].size - file_table[FD_DISPINFO].open_offset;
			dispinfo_read(buf,file_table[FD_DISPINFO].open_offset,len);
			file_table[FD_DISPINFO].open_offset+=len;
			Log("filesize after read:%d",file_table[FD_DISPINFO].size);
			return len;
		case FD_EVENTS:
			return events_read(buf,0,len);
		default:	
			if(file_table[fd].open_offset==file_table[fd].size)
				return 0;
			else if(file_table[fd].open_offset+len>file_table[fd].size)
				len = file_table[fd].size - file_table[fd].open_offset;
			//Log("%d",len);
			ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
			file_table[fd].open_offset+=len;
			//Log("%s",file_table[fd].name);
			return len;
	}
	*/
	ssize_t size,newread;
	size = file_table[fd].size - file_table[fd].open_offset;
	newread = len>size?size:len;
	if(file_table[fd].read!=NULL){
		file_table[fd].open_offset+=len;
		return (*file_table[fd].read)(buf,file_table[fd].disk_offset+file_table[fd].open_offset-len,len);
	}
	if(fd<6||fd>NR_FILES)
		return -1;
	ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,newread);
	file_table[fd].open_offset+=newread;
	return newread;
}

ssize_t fs_write(int fd,const void *buf,size_t len){
	/*
	switch(fd){
		case FD_STDOUT:
		case FD_STDERR:
		{
			char *buff = (char *)buf;
			for(int i=0;i<len;i++)
				_putc(buff[i]);
			return len;
		}
		case FD_FB:
			fb_write(buf,file_table[FD_FB].open_offset,len);
			file_table[FD_FB].open_offset+=len;
			return len;
		default:
			//assert(0);
			if(file_table[fd].open_offset==file_table[fd].size)
				return 0;
			if(file_table[fd].open_offset+len>file_table[fd].size)
				len = file_table[fd].size - file_table[fd].open_offset;
			ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
			file_table[fd].open_offset +=len;
			return len;
	}
	return 0;
	*/
	size_t size,newrite;
	size = file_table[fd].size-file_table[fd].open_offset;
	newrite = len>size?size:len;
	if(file_table[fd].write!=NULL){
		size_t res =0;
		res = (*file_table[fd].write)(buf,file_table[fd].disk_offset+file_table[fd].open_offset,newrite);
		file_table[fd].open_offset+=newrite;
		return res;
	}
	ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,newrite);
	file_table[fd].open_offset+=newrite;
	return newrite;
}

off_t fs_lseek(int fd,off_t offset,int whence){
	switch(whence){
		case SEEK_SET: 
			if(offset<0 || offset>file_table[fd].size)
				return -1;
			file_table[fd].open_offset = offset;
			return file_table[fd].open_offset;
		case SEEK_CUR:
			if(file_table[fd].open_offset+offset<0||file_table[fd].open_offset+offset>file_table[fd].size)
				return -1;
			file_table[fd].open_offset += offset;
			return file_table[fd].open_offset;
		case SEEK_END:
			if(file_table[fd].size+offset<0)
				return -1;
			file_table[fd].open_offset = file_table[fd].size+offset;
			//Log("%s",file_table[fd].name);
			//Log("%d",file_table[fd].size);
			//Log("%d",file_table[fd].open_offset);
			//assert(0);
			return file_table[fd].open_offset;
		default:
			assert(0);
			return 0;
 	}
}
int fs_close(int fd){
	file_table[fd].open_offset = 0;
	return 0;
}

size_t fs_filesz(int fd){
	return file_table[fd].size;
}


