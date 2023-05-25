/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#define CAPLDLL_EXPORTS
#include <stdio.h>
#include <stdint.h>
#include "minilogger.h"
#include "crc.h"
#include "filepraser.h"
#include "capldll.h"


/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int32_t 
 */

int32_t main(int32_t argc, char * argv[])
{
    uint32_t segmentsCount;
    uint8_t addressAndSize[50][8];
    uint8_t checksum[50][4];
    blOpenFlashFile(argv[1],&segmentsCount,addressAndSize,checksum);
}