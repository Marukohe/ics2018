#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
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

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"/proc/dispinfo", 0, 0, 0, dispinfo_read, invalid_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode){
  //printf("fs_open: name:%s\n", pathname);
  for(int i = 0;i < NR_FILES;i++){
    if(strcmp(pathname, file_table[i].name) == 0){
      return i;
    }
  }
  panic("fs_open: Cannot find the file.\n");
}

void init_fs() {
  for(int i = 0;i < NR_FILES;i++){
    file_table[i].open_offset = 0;
  }
  // TODO: initialize the size of /dev/fb
  int fb = fs_open("/dev/fb", 0, 0);
  file_table[fb].size = screen_width()*screen_height()*4;
}

size_t fs_filesz(int fd){
  return file_table[fd].size;
}



size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t fs_read(int fd, void *buf,  size_t len){
  if(file_table[fd].read != NULL){
    file_table[fd].open_offset += len;
    return (*file_table[fd].read)(buf,file_table[fd].disk_offset+file_table[fd].open_offset-len, len);
  }
  if(file_table[fd].open_offset+len > fs_filesz(fd)){
    len = fs_filesz(fd)-file_table[fd].open_offset;
  }
  ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
  file_table[fd].open_offset += len;
  return len;
}

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t fs_write(int fd, const void *buf, size_t len){
  if(file_table[fd].write != NULL){
size_t term = (*file_table[fd].write)(buf,file_table[fd].disk_offset+file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
    return term;
  }
  if(file_table[fd].open_offset+len > fs_filesz(fd)){
    len = fs_filesz(fd)-file_table[fd].open_offset;
    if(len < 0)
      return 0;
  }
  ramdisk_write(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  switch(whence){
    case SEEK_SET:
      file_table[fd].open_offset = offset;
      break;
    case SEEK_CUR:
      file_table[fd].open_offset += offset;
      break;
    case SEEK_END:
      file_table[fd].open_offset = file_table[fd].size + offset;
      break;
  }
  return file_table[fd].open_offset;
}

int fs_close(int fd){
  file_table[fd].open_offset = 0;
  return 0;
}

