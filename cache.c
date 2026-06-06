#define TOTAL_CACHE_SIZE 4 * 2^10
#define CACHE_LINE_SIZE  16
#define CACHE_LINE_COUNT (TOTAL_CACHE_SIZE / CACHE_LINE_SIZE)

#define DEBUG_MODE 1
#define DEBUG(...) { if (DEBUG_MODE) printf(__VA_ARGS__); }

#include "core.h"

typedef struct {
  u16 tag;
  u8  valid;
  u8  dirty;
  u8  bytes[CACHE_LINE_SIZE];
} Line;

typedef struct {
  Line lines[CACHE_LINE_COUNT];
} Cache;

// ---

u8 cache_read(Cache cache, u16 addr, u8 bytes[CACHE_LINE_SIZE]) {
  u16 index = addr % CACHE_LINE_COUNT;
  u16   tag = addr / CACHE_LINE_COUNT;
  Line line = cache.lines[index];

  DEBUG("-- READING\n\n");
  DEBUG("Address 0x%04x (%d)\n", addr, addr);
  DEBUG("Tag     0x%04x (%d)\n", tag, tag);
  DEBUG("Index   0x%04x (%d)\n", index, index);
  DEBUG("\n Valid | Dirty | Tag        | Data\n");
  DEBUG(" %d     |", line.valid);
  DEBUG(" %d     |", line.dirty);
  DEBUG(" 0x%04x (%d) | ", line.tag, line.tag);
  for (u8 i = 0; i < CACHE_LINE_SIZE; i++) DEBUG("0x%x ", line.bytes[i]);
  DEBUG("\n\n");

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

// ---

int main() {
  Cache c = { 0 };
  u8 result[CACHE_LINE_SIZE];

  cache_read(c, 1, result);

  return 0;
}
