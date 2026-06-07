#ifndef CACHE_H
#define CACHE_H

#include "core.h"
#include "ui.h"

// ---

u8 cache_read(Cache cache, u16 addr, u8 bytes[CACHE_LINE_SIZE]) {
  u16 index = addr % CACHE_LINE_COUNT;
  u16   tag = addr / CACHE_LINE_COUNT;
  Line line = cache.lines[index];

  DEBUG("-- READING\n\n");
  DEBUG("Address 0x%04x (%d)\n", addr, addr);
  DEBUG("Tag     0x%04x (%d)\n", tag, tag);
  DEBUG("Index   0x%04x (%d)\n", index, index);
  print_cache_header();
  print_cache_line(line, 0);
  DEBUG("\n");

  if (!line.valid) {
    DEBUG("Cache miss due to invalid line\n");
    return 0;
  }

  if (tag != line.tag) {
    DEBUG("Cache miss due to unmatched tags\n");
    return 0;
  }

  DEBUG("CACHE HIT\n");
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) bytes[i] = line.bytes[i];
  return 1;
}

void cache_write(Cache* cache, u16 addr, u8 bytes[CACHE_LINE_SIZE]) {
  u16 index = addr % CACHE_LINE_COUNT;
  u16   tag = addr / CACHE_LINE_COUNT;
  Line* line = &cache->lines[index];

  DEBUG("[WRITING]\n");
  DEBUG("Addres 0x%04x (%d)\n", addr, addr);
  DEBUG("Tag    0x%04x (%d)\n", tag, tag);
  DEBUG("Index  0x%04x (%d)\n", index, index);
  DEBUG("Data   ");
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) DEBUG("%d ", bytes[i]);
  DEBUG("\n");

  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) line->bytes[i] = bytes[i];
  line->tag   = tag;
  line->valid = 1;
  line->dirty = 0;
}

#endif
