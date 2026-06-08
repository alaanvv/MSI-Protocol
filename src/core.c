#include "core.h"

const char* BusReqView[32] = {
  "BusRd", "BusRdWr", "BusWr"
};

Cache cores[4] = { 0 };

u32 total_reads[4] = { 0 };
u32 read_errors[4] = { 0 };
u32 total_writes[4] = { 0 };
u32 write_errors[4] = { 0 };
u32 total_force_invalidations = 0;
u32 total_write_backs = 0;
