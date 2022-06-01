#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "minilogger.h"

typedef enum  {
CRC8=0x8,
CRC16=0x16,
CRC32=0x32
} crcWidth;

uint32_t appSepcifyCRCParameters();
uint8_t Reflect8(uint8_t val);
uint16_t Reflect16(uint16_t val);
uint32_t Reflect32(uint32_t val);
int32_t CalculateCrcTable_CRC8();
int32_t CalculateCrcTable_CRC16();
uint8_t CalculateCrcTable_CRC32();
int32_t CalculateCrcTable();
uint8_t Calculate_CRC8(const char* fileName);
uint16_t Calculate_CRC16(const char* fileName);
uint32_t Calculate_CRC32(const char* fileName);
uint32_t CalculateCrc(const char* fileName);

#endif
