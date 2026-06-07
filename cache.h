#ifndef CACHE_H
#define CACHE_H

#include "core.h"
#include "ui.h"

// ---

u8 cache_read(Cache cache, u16 addr) {
  u16  index = addr / CACHE_LINE_SIZE % CACHE_LINE_COUNT;
  u16    tag = addr / CACHE_LINE_SIZE / CACHE_LINE_COUNT;

  Line line = cache.lines[index];
  u8    hit = line.valid && tag == line.tag;

  printf("--  READING  --\n");
  printf("Addr 0x%04x (%d)\n", addr, addr);
  printf("Tag  0x%04x (%d)\n", tag, tag);
  printf("Idx  0x%04x (%d)\n", index, index);
  print_cache_line(line, index);

  return hit;
}

void cache_write(Cache* cache, u16 addr) {
  u16  index = addr / CACHE_LINE_SIZE % CACHE_LINE_COUNT;
  u16    tag = addr / CACHE_LINE_SIZE / CACHE_LINE_COUNT;

  Line* line = &cache->lines[index];

  printf("--  WRITING  --\n");
  printf("Addr 0x%04x (%d)\n", addr, addr);
  printf("Tag  0x%04x (%d)\n", tag, tag);
  printf("Idx  0x%04x (%d)\n", index, index);
  print_cache_line(*line, index);

  line->tag   = tag;
  line->valid = 1;
  line->dirty = 0;

  print_cache_line(*line, index);
}

#endif
