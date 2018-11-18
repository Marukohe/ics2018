#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  ReadFn read;
  WriteFn write;
  off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname,int flags,int mode){
 	for(int i=0;i<NR_FILES;i++){
		if(strcmp(file_table[i].name,pathname)==0)
			return i;
	}
	assert(0);
	return 0;
}
extern size_t ramdisk_read(void *buf,size_t offset,size_t len);
extern size_t ramdisk_write(const void *buf,size_t offset,size_t len);

ssize_t fs_read(int fd,void *buf,size_t len){
	if(file_table[fd].open_offset==file_table[fd].size)
		return 0;
	else if(file_table[fd].open_offset+len>file_table[fd].size)
		len = file_table[fd].size - file_table[fd].open_offset;
	ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
	file_table[fd].open_offset+=len;
	return len;
}

ssize_t fs_write(int fd,const void *buf,size_t len){
	switch(fd){
		case FD_STDOUT:
		case FD_STDERR:
		{
			char *buff = (char *)buf;
			for(int i=0;i<len;i++)
				_putc(buff[i]);
			return len;
		}
		default:
			if(file_table[fd].open_offset==file_table[fd].size)
				return 0;
			if(file_table[fd].open_offset+len>file_table[fd].size)
				len = file_table[fd].size - file_table[fd].open_offset;
			ramdisk_write(buf,file_table[fd].open_offset,len);
			file_table[fd].open_offset +=len;
			return len;
	}
	return 0;
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
			return file_table[fd].open_offset;
		default:
			assert(0);
			return 0;
 	}
}
int fs_close(int fd){
	return 0;
}

size_t fs_filesz(int fd){
	return file_table[fd].size;
}


