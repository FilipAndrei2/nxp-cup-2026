#ifndef MAIN_TYPES_H
#define MAIN_TYPES_H
/* Stub main_types.h for host-side unit testing */
#ifdef __cplusplus
extern "C" {
#endif

#include "PlatformTypes.h"

typedef struct {
    uint16 x0, y0, x1, y1;
    uint8  VectorIndex;
} Vector;

typedef struct {
    uint8  NumberOfVectors;
    Vector Vectors[100];
} DetectedVectors;

#ifdef __cplusplus
}
#endif
#endif /* MAIN_TYPES_H */
