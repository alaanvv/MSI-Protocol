#ifndef UI_H
#define UI_H

#include "core.h"
#include <string.h>

void print_line_content(Line line) {
  printf("│ %c │ 0x%04x │\n", line.state, line.tag);
}

void print_line(Line line) {
  printf("┌───┬────────┐\n");
  print_line_content(line);
  printf("└───┴────────┘\n");
}

void print_cache(Cache cache) {
  printf("┌───┬────────┐\n");
  for (u32 l = 0; l < CACHE_LINE_COUNT; l++) print_line_content(cache.lines[l]);
  printf("└───┴────────┘\n");
}

void print_core(Cache cache, u8 core_id) {
  printf("┌───CORE %d───┐\n", core_id);
  for (u32 l = 0; l < CACHE_LINE_COUNT; l++) print_line_content(cache.lines[l]);
  printf("└───┴────────┘\n");
}

void print_processor(Cache caches[4]) {
  for (u8 c = 0; c < CACHE_COUNT; c++) print_core(caches[c], c);
}

void print_processor_h(Cache caches[4]) {
  char ascii[CACHE_LINE_COUNT + 3][CACHE_COUNT * 32] = { 0 };
  char buffer[64];

  for (u8 c = 0; c < CACHE_COUNT; c++) {
    sprintf(buffer, "┌───CORE %d───┐", c);
    strcat(ascii[0], buffer);
  }

  for (u8 c = 0; c < CACHE_COUNT; c++) {
    for (u32 l = 0; l < CACHE_LINE_COUNT; l++) {
      sprintf(buffer, "│ %c │ 0x%04x │", caches[c].lines[l].state, caches[c].lines[l].tag);
      strcat(ascii[1 + l], buffer);
    }
  }

  for (u8 c = 0; c < CACHE_COUNT; c++) {
    sprintf(buffer, "└───┴────────┘");
    strcat(ascii[CACHE_LINE_COUNT + 1], buffer);
  }

  for (u8 i = 0; i < CACHE_LINE_COUNT + 2; i++)
    printf("%s\n", ascii[i]);
}

void print_address(u16 addr) {
  u16    tag = addr / CACHE_LINE_SIZE / CACHE_LINE_COUNT;
  u16  index = addr / CACHE_LINE_SIZE % CACHE_LINE_COUNT;

  printf("Address  0x%04x (%d)\n", addr, addr);
  printf("├── Tag  0x%04x (%d)\n", tag, tag);
  printf("└── Idx  0x%04x (%d)\n", index, index);
}

#endif
