#pragma once
/* Minimal AUTOSAR PlatformTypes stubs for host-side unit testing */
#include <stdint.h>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t   sint8;
typedef int16_t  sint16;
typedef int32_t  sint32;
typedef int64_t  sint64;
typedef float    float32;
typedef double   float64;
typedef uint8_t  boolean;

#ifndef TRUE
#define TRUE  1U
#endif
#ifndef FALSE
#define FALSE 0U
#endif
