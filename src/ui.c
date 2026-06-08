#include "ui.h"

void print_proc(Core cores[4]) {
  char ascii[LINE_AMOUNT + 3][CORE_AMOUNT * 32] = { 0 };
  char buffer[64];

  for (u8 c = 0; c < CORE_AMOUNT; c++)
    printf("┌───CORE %d───┐", c);
  printf("\n");

  for (u32 l = 0; l < LINE_AMOUNT; l++) {
    for (u8 c = 0; c < CORE_AMOUNT; c++)
      printf("│ %c │ 0x%04x │", cores[c].cache.lines[l].state, cores[c].cache.lines[l].tag);
    printf("\n");
  }

  for (u8 c = 0; c < CORE_AMOUNT; c++)
    printf("└───┴────────┘");
  printf("\n");
}

void print_addr(u16 addr) {
  u16    tag = addr / LINE_SIZE / LINE_AMOUNT;
  u16  index = addr / LINE_SIZE % LINE_AMOUNT;

  printf("Address  0x%04x (%d)\n", addr, addr);
  printf("├── Tag  0x%04x (%d)\n", tag, tag);
  printf("└── Idx  0x%04x (%d)\n", index, index);
}
