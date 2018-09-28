#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /*struct watchpoint *prev;*/
  /* TODO: Add more members if necessary */
  char bufs[65536];
  uint32_t ans;
} WP;

WP* new_wp();
void free_wp(int n);
void init_wp_pool();
void pwatchpoint();
bool checkchange();

#endif
