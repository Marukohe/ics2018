#include <am.h>

void print(const char *s) {
  for (; *s; s ++) {
    _putc(*s);
  }
}
int main() {
  for (int i = 0; i < 10; i ++) {
    print("Hello World!\n");
  /*test	*/
  //int a = 10;
  //print("%02d\n",a);
  }
  return 0;
}
