#ifndef UI_H
#define UI_H

#include "core.h"

void print_line(Line line) {
  printf("┌───┬────────┐\n");
  printf("│ %c │ 0x%04x │\n", line.state, line.tag);
  printf("└───┴────────┘\n");
}

#endif
