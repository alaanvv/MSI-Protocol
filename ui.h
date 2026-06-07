#ifndef UI_H
#define UI_H

#include "core.h"

void print_cache_line(Line line, u8 index) {
  printf("%03d -> ", index);
  printf("%c | ", line.valid ? 'V' : 'I');
  printf("%c | ", line.dirty ? 'D' : 'C');
  printf("0x%04x\n", line.tag);
}

void print_cache(Cache cache) {
  for (u16 i = 0; i < CACHE_LINE_COUNT; i++)
    print_cache_line(cache.lines[i], i);
}

#endif
