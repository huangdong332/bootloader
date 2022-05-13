#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "minilogger.h"

uint32_t appSepcifyCRCParameters();
uint8_t Reflect8(uint8_t val);
uint32_t Reflect32(uint32_t val);
uint8_t CalculateCrcTable_CRC32();
uint32_t Calculate_CRC32(const char* fileName);

#endif