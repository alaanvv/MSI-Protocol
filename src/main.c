#include "cache.h"

int main() {
  for (u8 c = 0; c < 4; c++)
    for (u32 l = 0; l < LINE_AMOUNT; l++)
      cores[c].cache.lines[l].state = 'I';

  if (CACHE_VIEW_MODE)
    print_processor_h(cores);

  char buffer[64];
  FILE* file = fopen("in/TRACE.txt", "r");
  u8 core_id;
  char op;
  u16 addr;
  while (fgets(buffer, 64, file)) {
    sscanf(buffer, "CORE %hhd %c%*s %hx", &core_id, &op, &addr);

    DEBUG("%s", buffer);

    if (CACHE_VIEW_MODE) {
      printf("\033[2J\033[H");
      fflush(stdout);
      print_processor_h(cores);
      printf("\n> %s", buffer);
      print_address(addr);
    }

    if (INTERACTIVE_MODE) {
      getchar();
    }

    Cache* cache = &cores[core_id].cache;
    if (op == 'R') {
      DEBUG("REQUISIÇÃO DE PROCESSADOR: PrRd no endereço 0x%04x\n", addr);
      cache_rd(cache, core_id, addr);
    }
    else {
      DEBUG("REQUISIÇÃO DE PROCESSADOR: PrWr no endereço 0x%04x\n", addr);
      cache_wr(cache, core_id, addr);
    }
  }

  u32 total_force_invalidations = 0;
  u32 total_write_backs = 0;
  for (u8 c = 0; c < CORE_AMOUNT; c++) {
    DEBUG("TAXA DE ERROS DE LEITURA DO CORE %d: %f\n", c, (f32) cores[c].cache.rd_err_count / cores[c].cache.rd_count);
    DEBUG("TAXA DE ERROS DE ESCRITA DO CORE %d: %f\n", c, (f32) cores[c].cache.wr_err_count / cores[c].cache.wr_count);
    total_force_invalidations += cores[c].cache.force_invalidation_count;
    total_write_backs += cores[c].cache.write_back_count;
  }

  DEBUG("INVALIDAÇÕES FORÇADAS: %d\n", total_force_invalidations);
  DEBUG("WRITE-BACKS: %d\n", total_write_backs);

  if (INTERACTIVE_MODE) {
    getchar();
  }

  return 0;
}
