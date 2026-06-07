#ifndef CACHE_H
#define CACHE_H

#include "core.h"
#include "ui.h"

// ---

u8 cache_read(Cache cache, u16 addr, u8 bytes[CACHE_LINE_SIZE]) {
  u16 index = addr % CACHE_LINE_COUNT;
  u16   tag = addr / CACHE_LINE_COUNT;
  Line line = cache.lines[index];

  printf("-- READING\n\n");
  printf("Address 0x%04x (%d)\n", addr, addr);
  printf("Tag     0x%04x (%d)\n", tag, tag);
  printf("Index   0x%04x (%d)\n", index, index);
  print_cache_header();
  print_cache_line(line, 0);
  printf("\n");

  if (!line.valid) {
    printf("Cache miss due to invalid line\n");
    return 0;
  }

  if (tag != line.tag) {
    printf("Cache miss due to unmatched tags\n");
    return 0;
  }

  printf("CACHE HIT\n");
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) bytes[i] = line.bytes[i];
  return 1;
}

void cache_write(Cache* cache, u16 addr, u8 bytes[CACHE_LINE_SIZE]) {
  u16 index = addr % CACHE_LINE_COUNT;
  u16   tag = addr / CACHE_LINE_COUNT;
  Line* line = &cache->lines[index];

  printf("[WRITING]\n");
  printf("Addres 0x%04x (%d)\n", addr, addr);
  printf("Tag    0x%04x (%d)\n", tag, tag);
  printf("Index  0x%04x (%d)\n", index, index);
  printf("Data   ");
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) printf("%d ", bytes[i]);
  printf("\n");

  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) line->bytes[i] = bytes[i];
  line->tag   = tag;
  line->valid = 1;
  line->dirty = 0;
}

#endif
