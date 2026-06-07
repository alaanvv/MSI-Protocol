#ifndef BUS_H
#define BUS_H

#include "core.h"
#include "cache.h"

void bus_sig(BusReq req, Cache core[4], u8 from_id, u16 addr) {
  if (!req) return;

  for (u8 c = 0; c < CACHE_COUNT; c++) {
    if (c == from_id) continue;
    cache_snoop(&core[c], c, req, addr);
  }
}

#endif
