#include <am.h>
#include <x86.h>
#include <amdev.h>

int p = 0x48;
static uint32_t boottime;

NDL_EVENT *event;

size_t timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
	  uint32_t nowtime=inl(p);
      _UptimeReg *uptime = (_UptimeReg *)buf;
      uptime->hi = 0;
      uptime->lo = nowtime-boottime;
	  event->type = NDL_EVENT_TIMER;
	  event->data = nowtime - boottime;
	  //NDL_WaitEvent(event);
      return sizeof(_UptimeReg);
    }
    case _DEVREG_TIMER_DATE: {
      _RTCReg *rtc = (_RTCReg *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2018;
      return sizeof(_RTCReg);
    }
  }
  return 0;
}

void timer_init() {
	boottime=inl(p);
}
