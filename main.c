#include "cache.h"

int main() {
  Cache cores[4] = { 0 };

  for (u8 c = 0; c < 4; c++)
    for (u32 l = 0; l < CACHE_LINE_COUNT; l++)
      cores[c].lines[l].state = 'I';

  FILE* file = fopen("TRACE.txt", "r");
  c8 buffer[64];

  u8 i = 0;
  while (fgets(buffer, 64, file) && i++ < 10) {
    u8 core_id;
    char op;
    u16 addr;

    sscanf(buffer, "CORE %hhd %c%*s %hx", &core_id, &op, &addr);

    Cache* cache = &cores[core_id];
    if (op == 'R') cache_rd(cache, addr);
    else           cache_wr(cache, addr);
  }

  return 0;
}
