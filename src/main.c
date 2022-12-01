#define CAPLDLL_EXPORTS
#include <stdio.h>
#include <stdint.h>
#include "minilogger.h"
#include "crc.h"
#include "filepraser.h"
#include "capldll.h"


int32_t main(int32_t argc, char * argv[])
{
    uint32_t segmentsCount;
    uint8_t addressAndSize[5][8];
    uint8_t checksum[5][4];
    blOpenFlashFile(argv[1],&segmentsCount,addressAndSize,checksum);
}