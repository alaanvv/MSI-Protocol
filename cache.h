#ifndef CACHE_H
#define CACHE_H

#include "core.h"
#include "ui.h"

// ---

u8 cache_read(Cache cache, u16 addr) {
  u16 offset = addr % CACHE_LINE_SIZE;
  u16 index  = addr / CACHE_LINE_SIZE % CACHE_LINE_COUNT;
  u16   tag  = addr / CACHE_LINE_SIZE / CACHE_LINE_COUNT;
  Line line  = cache.lines[index];

  printf("-- READING\n\n");
  printf("Address 0x%04x (%d)\n", addr, addr);
  printf("Tag     0x%04x (%d)\n", tag, tag);
  printf("Index   0x%04x (%d)\n", index, index);
  printf("Offset  0x%04x (%d)\n\n", offset, offset);
  print_cache_header();
  print_cache_line(line, offset);
  printf("\n");

  if (!line.valid || tag != line.tag) {
    printf("Cache miss\n");
    return 0;
  }

  printf("Cache hit\n");
  return 1;
}

void cache_write(Cache* cache, u16 addr) {
  u16 index = addr % CACHE_LINE_COUNT;
  u16   tag = addr / CACHE_LINE_COUNT;
  Line* line = &cache->lines[index];

  printf("[WRITING]\n");
  printf("Addres 0x%04x (%d)\n", addr, addr);
  printf("Tag    0x%04x (%d)\n", tag, tag);
  printf("Index  0x%04x (%d)\n", index, index);

  line->tag   = tag;
  line->valid = 1;
  line->dirty = 0;
}

#endif
