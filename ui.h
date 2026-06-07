#ifndef UI_H
#define UI_H

#include "core.h"

void print_cache_header() {
  printf("         Valid | Dirty | Tag    | Data\n");
}

void print_cache_line(Line line, u8 index) {
  printf(" %03d -> ", index);
  printf(" %d     |", line.valid);
  printf(" %d     |", line.dirty);
  printf(" 0x%04x | ", line.tag);
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) printf("0x%x ", line.bytes[i]);
  printf("\n");
}

void print_cache(Cache cache) {
  print_cache_header();
  for (u16 i = 0; i < CACHE_LINE_COUNT; i++) print_cache_line(cache.lines[i], i);
}

#endif
