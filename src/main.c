#include "cache.h"

int main() {
  Cache cores[4] = { 0 };

  for (u8 c = 0; c < 4; c++)
    for (u32 l = 0; l < CACHE_LINE_COUNT; l++)
      cores[c].lines[l].state = 'I';

  c8 buffer[64];
  FILE* file = fopen("in/TRACE.txt", "r");
  while (fgets(buffer, 64, file)) {
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
