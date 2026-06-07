#ifndef CACHE_H
#define CACHE_H

#include "core.h"
#include "ui.h"

// ---

void bus_sig(BusReq req, u8 from_id, u16 addr);
void cache_snoop(Cache* cache, u8 core_id, BusReq req, u16 addr);

// ---

u8 tag_lookup(Cache* cache, u16 addr, u16* tag_out, u16* index_out) {
  u16    tag = addr / CACHE_LINE_SIZE / CACHE_LINE_COUNT;
  u16  index = addr / CACHE_LINE_SIZE % CACHE_LINE_COUNT;
  Line* line = &cache->lines[index];

  if (tag_out)   *tag_out   = tag;
  if (index_out) *index_out = index;

  u8 hit = line->state != 'I';

  DEBUG("Address  0x%04x (%d)\n", addr, addr);
  DEBUG("├── Tag  0x%04x (%d)\n", tag, tag);
  DEBUG("└── Idx  0x%04x (%d)\n", index, index);
  if (DEBUG_MODE) print_line(*line);

  if (hit) DEBUG("Cache hit\n");
  else     DEBUG("Cache miss\n");

  return hit;
}

void cache_rd(Cache* cache, u8 core_id, u16 addr) {
  DEBUG("--  READING  --\n");

  u16 tag, index;
  u8 hit = tag_lookup(cache, addr, &tag, &index);

  Line* line = &cache->lines[index];

  if (!hit) {
    bus_sig(BUS_RD, core_id, addr);
    line->tag   = tag;
    line->state = 'S';
    if (DEBUG_MODE) print_line(*line);
  }
}

void cache_wr(Cache* cache, u8 core_id, u16 addr) {
  DEBUG("--  WRITING  --\n");

  u16 tag, index;
  u8 hit = tag_lookup(cache, addr, &tag, &index);

  Line* line = &cache->lines[index];

  if (hit) {
    if (line->state == 'S')
      bus_sig(BUS_WR, core_id, addr);
    line->tag   = tag;
    line->state = 'M';
    if (DEBUG_MODE) print_line(*line);
  }
  else {
    bus_sig(BUS_RD_WR, core_id, addr);
    line->tag   = tag;
    line->state = 'M';
    if (DEBUG_MODE) print_line(*line);
  }
}

void bus_sig(BusReq req, u8 from_id, u16 addr) {
  for (u8 c = 0; c < CACHE_COUNT; c++) {
    if (c == from_id) continue;
    cache_snoop(&cores[c], c, req, addr);
  }
}

void cache_snoop(Cache* cache, u8 core_id, BusReq req, u16 addr) {
  if (req == BUS_RD) {
    u16 index;
    u8 hit = tag_lookup(cache, addr, NULL, &index);
    if (!hit) return;

    char state = cache->lines[index].state;
    if (state != 'M') return;

    cache->lines[index].state = 'S';
    DEBUG("Data found on core %d\n", core_id);
  }
  if (req == BUS_WR) {
    u16 index;
    u8 hit = tag_lookup(cache, addr, NULL, &index);
    if (!hit) return;

    cache->lines[index].state = 'I';
  }
  if (req == BUS_RD_WR) {
    u16 index;
    u8 hit = tag_lookup(cache, addr, NULL, &index);
    if (!hit) return;

    char state = cache->lines[index].state;
    if (state == 'M') DEBUG("Data found on core %d\n", core_id);

    cache->lines[index].state = 'I';
  }
}

#endif
