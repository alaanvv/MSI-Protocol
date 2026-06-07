#ifndef CORE_H
#define CORE_H

#define PRINT(...) { printf(__VA_ARGS__); printf("\n"); }

#define CACHE_COUNT 4
#define TOTAL_CACHE_SIZE 4096
#define CACHE_LINE_SIZE  16
#define CACHE_LINE_COUNT (TOTAL_CACHE_SIZE / CACHE_LINE_SIZE)

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
  PR_RD, PR_WR
} CacheReq;

typedef enum {
  BUS_RD, BUS_RD_WR, BUS_WR
} BusReq;

typedef struct {
  char state;
  u16  tag;
} Line;

typedef struct {
  Line lines[CACHE_LINE_COUNT];
} Cache;

Cache cores[4] = { 0 };

#endif
