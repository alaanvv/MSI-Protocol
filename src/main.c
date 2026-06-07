#include "cache.h"

int main() {
  for (u8 c = 0; c < 4; c++)
    for (u32 l = 0; l < CACHE_LINE_COUNT; l++)
      cores[c].lines[l].state = 'I';

  if (INTERACTIVE_MODE)
    print_processor_h(cores);

  c8 buffer[64];
  FILE* file = fopen("in/SMALL_TRACE.txt", "r");
  u8 core_id;
  char op;
  u16 addr;
  while (fgets(buffer, 64, file)) {
    sscanf(buffer, "CORE %hhd %c%*s %hx", &core_id, &op, &addr);

    if (INTERACTIVE_MODE) {
      printf("\n> %s", buffer);
      print_address(addr);

      getchar();
      printf("\033[2J\033[H");
      fflush(stdout);
    }

    Cache* cache = &cores[core_id];
    if (op == 'R') cache_rd(cache, core_id, addr);
    else           cache_wr(cache, core_id, addr);

    if (INTERACTIVE_MODE)
      print_processor_h(cores);
  }


  return 0;
}
