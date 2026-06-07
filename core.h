#ifndef CORE_H
#define CORE_H

#define TOTAL_CACHE_SIZE 4096
#define CACHE_LINE_SIZE  16
#define CACHE_LINE_COUNT (TOTAL_CACHE_SIZE / CACHE_LINE_SIZE)

#define PRINT(...) { printf(__VA_ARGS__); printf("\n"); }

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

typedef struct {
  u16 tag;
  u8  valid;
  u8  dirty;
} Line;

typedef struct {
  Line lines[CACHE_LINE_COUNT];
} Cache;

#endif
