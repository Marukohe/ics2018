#include <am.h>
#include <x86.h>
#include <amdev.h>
#define KEY_MASK 0x8000

int p1=0x60;
size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
	  uint32_t getkeybord = inl(p1);
      _KbdReg *kbd = (_KbdReg *)buf;
      kbd->keydown = (getkeybord & KEY_MASK?1:0);
      kbd->keycode = (getkeybord)&~KEY_MASK;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
