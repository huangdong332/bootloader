#ifndef CAPLDLL_H
#define CAPLDLL_H
#include <stdint.h>

int32_t __declspec(dllimport) __stdcall blOpenFlashFile(const char *fileName,
                                              uint32_t *segmentsCount, uint8_t addressAndSize[][11],
                                              uint8_t checksum[][8]);
int32_t __declspec(dllimport) __stdcall blBuffer(uint32_t bufferLength,
uint8_t *data, uint32_t *dataLength, uint32_t segment);

#endif