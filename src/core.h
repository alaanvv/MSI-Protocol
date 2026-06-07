#ifndef CORE_H
#define CORE_H

#define PRINT(...) { printf(__VA_ARGS__); printf("\n"); }

#define CACHE_COUNT 4
#define TOTAL_CACHE_SIZE 4096
#define CACHE_LINE_SIZE  16
#define CACHE_LINE_COUNT (TOTAL_CACHE_SIZE / CACHE_LINE_SIZE)

#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#endif
#ifndef INTERACTIVE_MODE 
#define INTERACTIVE_MODE 0
#endif
#ifndef CACHE_VIEW_MODE 
#define CACHE_VIEW_MODE 0
#endif
#define DEBUG(...) (DEBUG_MODE ? printf(__VA_ARGS__) : 0)

#include <stdint.h>
#include <stdio.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef float    f32;
typedef double   f64;
typedef char     c8;

typedef enum {
  BUS_RD, BUS_RD_WR, BUS_WR
} BusReq;

char* BusReqView[32] = {
  "BusRd",
  "BusRdWr",
  "BusWr",
};

typedef struct {
  char state;
  u16  tag;
} Line;

typedef struct {
  Line lines[CACHE_LINE_COUNT];
} Cache;

Cache cores[4] = { 0 };

i8 total_reads[4];
i8 read_errors[4];
i8 total_writes[4];
i8 write_errors[4];
i8 total_force_invalidations;
i8 total_write_backs;

#endif
