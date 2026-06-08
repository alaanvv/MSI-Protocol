#ifndef CACHE_H
#define CACHE_H

#include "core.h"

void cache_rd(Cache* cache, u8 core_id, u16 addr);
void cache_wr(Cache* cache, u8 core_id, u16 addr);

#endif
