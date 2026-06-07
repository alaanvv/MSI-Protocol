#ifndef CACHE_H
#define CACHE_H

#include "core.h"
#include "ui.h"

// ---

u8 tag_lookup(Cache* cache, u16 addr, u16* tag_out, u16* index_out) {
  u16    tag = addr / CACHE_LINE_SIZE / CACHE_LINE_COUNT;
  u16  index = addr / CACHE_LINE_SIZE % CACHE_LINE_COUNT;
  Line* line = &cache->lines[index];

  if (tag_out)   *tag_out   = tag;
  if (index_out) *index_out = index;

  u8 hit = line->state != 'I';

  printf("Address  0x%04x (%d)\n", addr, addr);
  printf("├── Tag  0x%04x (%d)\n", tag, tag);
  printf("└── Idx  0x%04x (%d)\n", index, index);
  print_line(*line);

  if (hit) printf("Cache hit\n");
  else     printf("Cache miss\n");

  return hit;
}

u8 cache_rd(Cache* cache, u16 addr) {
  printf("--  READING  --\n");

  u8 hit = tag_lookup(cache, addr, NULL, NULL);

  return hit;
}

u8 cache_wr(Cache* cache, u16 addr) {
  printf("--  WRITING  --\n");

  u16 tag, index;
  u8 hit = tag_lookup(cache, addr, &tag, &index);

  Line* line = &cache->lines[index];

  if (hit) {
    line->tag   = tag;
    line->state = 'M';
    print_line(*line);
  }

  return hit;
}

#endif
