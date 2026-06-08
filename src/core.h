#ifndef CORE_H
#define CORE_H

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
#define PRINT(...) { printf(__VA_ARGS__); printf("\n"); }

#define CACHE_SIZE  4096
#define LINE_SIZE   16
#define CORE_AMOUNT 4
#define LINE_AMOUNT (CACHE_SIZE / LINE_SIZE)

#include <stdint.h>
#include <stdio.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   i8;
typedef float    f32;

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
  Line lines[LINE_AMOUNT];
} Cache;

Cache cores[4] = { 0 };

u32 total_reads[4];
u32 read_errors[4];
u32 total_writes[4];
u32 write_errors[4];
u32 total_force_invalidations;
u32 total_write_backs;

#endif
