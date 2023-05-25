#ifndef FILEPRASER_H
#define FILEPRASER_H
#include <stdint.h>
#include <stdio.h>
#include "minilogger.h"
#include "crc.h"
#ifdef __cplusplus
extern "C" {
#endif
uint8_t AscCodedHex2Buffer(const char * ascCodedHex, uint8_t * destinationBuffer);
uint8_t Uint2Array(uint32_t * targetUint, uint8_t * destinationArray);
uint8_t HandleHex(const char *fileName, uint32_t *segmentsCount, uint8_t addressAndSize[][8], uint8_t checksum[][4]);
uint8_t HandleSREC(const char *fileName, uint32_t *segmentsCount, uint8_t addressAndSize[][8], uint8_t checksum[][4]);
#ifdef __cplusplus
}
#endif
#endif