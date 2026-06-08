#ifndef CACHE_H
#define CACHE_H

#include "core.h"

void cache_rd(Core* core, u16 addr);
void cache_wr(Core* core, u16 addr);

#endif
