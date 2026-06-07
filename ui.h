#ifndef UI_H
#define UI_H

#include "core.h"

void print_cache_header() {
  DEBUG("         Valid | Dirty | Tag    | Data\n");
}

void print_cache_line(Line line, u8 index) {
  DEBUG(" %03d -> ", index);
  DEBUG(" %d     |", line.valid);
  DEBUG(" %d     |", line.dirty);
  DEBUG(" 0x%04x | ", line.tag);
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) DEBUG("0x%x ", line.bytes[i]);
  DEBUG("\n");
}

void print_cache(Cache cache) {
  print_cache_header();
  for (u16 i = 0; i < CACHE_LINE_COUNT; i++) print_cache_line(cache.lines[i], i);
}

#endif
