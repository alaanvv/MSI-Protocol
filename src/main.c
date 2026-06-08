#include "cache.h"

int main() {
  for (u8 c = 0; c < 4; c++)
    for (u32 l = 0; l < LINE_AMOUNT; l++)
      cores[c].lines[l].state = 'I';

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

    Cache* cache = &cores[core_id];
    if (op == 'R') {
      DEBUG("REQUISIÇÃO DE PROCESSADOR: PrRd no endereço 0x%04x\n", addr);
      cache_rd(cache, core_id, addr);
    }
    else {
      DEBUG("REQUISIÇÃO DE PROCESSADOR: PrWr no endereço 0x%04x\n", addr);
      cache_wr(cache, core_id, addr);
    }
  }

  for (u8 c = 0; c < CORE_AMOUNT; c++) {
    DEBUG("TAXA DE ERROS DE LEITURA DO CORE %d: %f\n", c, (f32) read_errors[c] / total_reads[c]);
    DEBUG("TAXA DE ERROS DE ESCRITA DO CORE %d: %f\n", c, (f32) write_errors[c] / total_writes[c]);
  }
  DEBUG("INVALIDAÇÕES FORÇADAS: %d\n", total_force_invalidations);
  DEBUG("WRITE-BACKS: %d\n", total_write_backs);

  if (INTERACTIVE_MODE) {
    getchar();
  }

  return 0;
}
