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

typedef struct {
  char state;
  u16  tag;
} Line;

typedef struct {
  Line lines[LINE_AMOUNT];
  u32 rd_count, rd_err_count;
  u32 wr_count, wr_err_count;
  u32 force_invalidation_count;
  u32 write_back_count;
} Cache;

typedef struct {
  Cache cache;
  u8 id;
} Core;

extern const char* BusReqView[32];

extern Core cores[4];

#endif
