#include <stdio.h>
#include <stdint.h>
#include "minilogger.h"
#include "crc.h"
#include "filepraser.h"
#include "capldll.h"

uint8_t TestSepcifyCRCParameters()
{
    extern crcWidth width;
    extern uint32_t polynomial;
    extern uint32_t initialValue;
    extern uint32_t finalXORValue;
    extern uint8_t inputReflected;
    extern uint8_t resultReflected;

    appSepcifyCRCParameters();
    if (width == CRC32 &&
        polynomial == 0x04C11DB7 &&
        initialValue == 0xFFFFFFFF &&
        finalXORValue == 0xFFFFFFFF &&
        inputReflected == 0x1 &&
        resultReflected == 0x1)
        log_info("TestSepcifyCRCParameters: pass");
    else
        log_info("TestSepcifyCRCParameters: fail");
    return 0;
}

uint8_t TestCalculateCrcTable_CRC8()
{
    extern uint32_t crcTable[256];
    extern uint32_t polynomial;

    polynomial=0x07;
    CalculateCrcTable_CRC8();
    if (crcTable[1] == polynomial)
        log_info("TestCalculateCrcTable_CRC8: pass");
    else
        log_info("TestCalculateCrcTable_CRC8: fail");
    return 0;
}

uint8_t TestCalculateCrcTable_CRC16()
{
    extern uint32_t crcTable[256];
    extern uint32_t polynomial;

    polynomial=0x1021;
    CalculateCrcTable_CRC16();
    if (crcTable[1] == polynomial)
        log_info("TestCalculateCrcTable_CRC16: pass");
    else
        log_info("TestCalculateCrcTable_CRC16: fail");
    return 0;
}

uint8_t TestCalculateCrcTable_CRC32()
{
    extern uint32_t crcTable[256];
    extern uint32_t polynomial;

    polynomial=0x04C11DB7;
    CalculateCrcTable_CRC32();
    if (crcTable[1] == polynomial)
        log_info("TestCalculateCrcTable_CRC32: pass");
    else
        log_info("TestCalculateCrcTable_CRC32: fail");
    return 0;
}

uint8_t TestReflect8()
{
    if (Reflect8(0xee) == 0x77)
        log_info("TestReflect8: pass");
    else
        log_info("TestReflect8: fail");
    return 0;
}

uint8_t TestReflect16()
{
    if (Reflect16(0xeeee) == 0x7777)
        log_info("TestReflect16: pass");
    else
        log_info("TestReflect16: fail");
    return 0;
}

uint8_t TestReflect32()
{
    if (Reflect32(0xeeeeeeee) == 0x77777777)
        log_info("TestReflect32: pass");
    else
        log_info("TestReflect32: fail");
    //    log_info("%.8X",Reflect32(0xeeee));
    return 0;
}

uint8_t TestCalculate_CRC8()
{
    extern crcWidth width;
    extern uint32_t polynomial;
    extern uint32_t initialValue;
    extern uint32_t finalXORValue;
    extern uint8_t inputReflected;
    extern uint8_t resultReflected;
    // CRC-32
    width=CRC8;
    polynomial = 0x07;
    initialValue = 0x0;
    finalXORValue = 0x0;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xF4000000)
        log_info("TestCalculate_CRC8 TC1: pass");
    else
        log_info("TestCalculate_CRC8 TC1: fail");
    // CRC-32/BZIP2
    width=CRC8;
    polynomial = 0x9B;
    initialValue = 0xFF;
    finalXORValue = 0x0;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xDA000000)
        log_info("TestCalculate_CRC8 TC2: pass");
    else
        log_info("TestCalculate_CRC8 TC2: fail");
    // CRC-32C
    width=CRC8;
    polynomial = 0x39;
    initialValue = 0x0;
    finalXORValue = 0x0;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0x15000000)
        log_info("TestCalculate_CRC8 TC3: pass");
    else
        log_info("TestCalculate_CRC8 TC3: fail");
    // CRC-32D
    width=CRC8;
    polynomial = 0xD5;
    initialValue = 0x0;
    finalXORValue = 0x0;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xBC000000)
        log_info("TestCalculate_CRC8 TC4: pass");
    else
        log_info("TestCalculate_CRC8 TC4: fail");

    return 0;
}

uint8_t TestCalculate_CRC16()
{
    extern crcWidth width;
    extern uint32_t polynomial;
    extern uint32_t initialValue;
    extern uint32_t finalXORValue;
    extern uint8_t inputReflected;
    extern uint8_t resultReflected;
    // CRC-32
    width=CRC16;
    polynomial = 0x1021;
    initialValue = 0xFFFF;
    finalXORValue = 0x0;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0x29B10000)
        log_info("TestCalculate_CRC16 TC1: pass");
    else
        log_info("TestCalculate_CRC16 TC1: fail");
    // CRC-32/BZIP2
    width=CRC16;
    polynomial = 0x8005;
    initialValue = 0x0;
    finalXORValue = 0x0;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xBB3D0000)
        log_info("TestCalculate_CRC16 TC2: pass");
    else
        log_info("TestCalculate_CRC16 TC2: fail");
    // CRC-32C
    width=CRC16;
    polynomial = 0x1021;
    initialValue = 0x1D0F;
    finalXORValue = 0x0;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xE5CC0000)
        log_info("TestCalculate_CRC16 TC3: pass");
    else
        log_info("TestCalculate_CRC16 TC3: fail");
    // CRC-32D
    width=CRC16;
    polynomial = 0x8005;
    initialValue = 0x0;
    finalXORValue = 0x0;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xFEE80000)
        log_info("TestCalculate_CRC16 TC4: pass");
    else
        log_info("TestCalculate_CRC16 TC4: fail");

    return 0;
}

uint8_t TestCalculate_CRC32()
{
    extern crcWidth width;
    extern uint32_t polynomial;
    extern uint32_t initialValue;
    extern uint32_t finalXORValue;
    extern uint8_t inputReflected;
    extern uint8_t resultReflected;
    // CRC-32
    width=CRC32;
    polynomial = 0x04C11DB7;
    initialValue = 0xFFFFFFFF;
    finalXORValue = 0xFFFFFFFF;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xCBF43926)
        log_info("TestCalculate_CRC32 TC1: pass");
    else
        log_info("TestCalculate_CRC32 TC1: fail");
    // CRC-32/BZIP2
    width=CRC32;
    polynomial = 0x04C11DB7;
    initialValue = 0xFFFFFFFF;
    finalXORValue = 0xFFFFFFFF;
    inputReflected = 0x0;
    resultReflected = 0x0;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xFC891918)
        log_info("TestCalculate_CRC32 TC2: pass");
    else
        log_info("TestCalculate_CRC32 TC2: fail");
    // CRC-32C
    width=CRC32;
    polynomial = 0x1EDC6F41;
    initialValue = 0xFFFFFFFF;
    finalXORValue = 0xFFFFFFFF;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0xE3069283)
        log_info("TestCalculate_CRC32 TC3: pass");
    else
        log_info("TestCalculate_CRC32 TC3: fail");
    // CRC-32D
    width=CRC32;
    polynomial = 0xA833982B;
    initialValue = 0xFFFFFFFF;
    finalXORValue = 0xFFFFFFFF;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable();
    if (CalculateCrc("crccheckdata") == 0x87315576)
        log_info("TestCalculate_CRC32 TC4: pass");
    else
        log_info("TestCalculate_CRC32 TC4: fail");

    return 0;
}

uint8_t TestHandleHex()
{
    uint32_t segmentsCount;
    uint8_t addressAndSize[5][11];
    uint8_t checksum[5][8];
    extern uint32_t polynomial;
    extern uint32_t initialValue;
    extern uint32_t finalXORValue;
    extern uint8_t inputReflected;
    extern uint8_t resultReflected;
    // CRC-32
    polynomial = 0x04C11DB7;
    initialValue = 0xFFFFFFFF;
    finalXORValue = 0xFFFFFFFF;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable_CRC32();
    HandleHex("test.HEX", &segmentsCount,
              addressAndSize, checksum);
    if (segmentsCount == 3)
        log_info("TestHandleHex TC1: pass");
    else
        log_info("TestHandleHex TC1: fail");
    if (addressAndSize[0][4] == 0x03 &&
        addressAndSize[0][10] == 0x40 &&
        addressAndSize[3][4] == 0x13 &&
        addressAndSize[3][10] == 0x70)
        log_info("TestHandleHex TC2: pass");
    else
        log_info("TestHandleHex TC2: fail");
    if(checksum[0][7]==0x86 &&
    checksum[3][7]==0xcd)
        log_info("TestHandleHex TC3: pass");
    else
        log_info("TestHandleHex TC3: fail");

    return 0;
}

uint8_t TestHandleSREC()
{
    uint32_t segmentsCount;
    uint8_t addressAndSize[5][11];
    uint8_t checksum[5][8];
    extern uint32_t polynomial;
    extern uint32_t initialValue;
    extern uint32_t finalXORValue;
    extern uint8_t inputReflected;
    extern uint8_t resultReflected;
    // CRC-32
    polynomial = 0x04C11DB7;
    initialValue = 0xFFFFFFFF;
    finalXORValue = 0xFFFFFFFF;
    inputReflected = 0x1;
    resultReflected = 0x1;
    CalculateCrcTable_CRC32();
    HandleSREC("test.S19", &segmentsCount,
               addressAndSize, checksum);
    if (segmentsCount == 3)
        log_info("TestHandleSREC TC1: pass");
    else
        log_info("TestHandleSREC TC1: fail");
    if (addressAndSize[0][4] == 0x03 &&
        addressAndSize[0][10] == 0x80 &&
        addressAndSize[3][4] == 0x0f &&
        addressAndSize[3][10] == 0x10)
        log_info("TestHandleSREC TC2: pass");
    else
        log_info("TestHandleSREC TC2: fail");
    if(checksum[0][7]==0x69 &&
    checksum[3][7]==0x8a)
        log_info("TestHandleSREC TC3: pass");
    else
        log_info("TestHandleSREC TC3: fail");
    return 0;
}

uint8_t TestAscCodedHex2Buffer()
{
    uint8_t data[10];
    AscCodedHex2Buffer("223344", data);
    if (data[0] == 0x22 &&
        data[1] == 0x33 &&
        data[2] == 0x44)
        log_info("TestAscCodedHex2Buffer: pass");
    else
        log_info("TestAscCodedHex2Buffer: fail");
    return 0;
}

uint8_t TestUint2Array()
{
    uint8_t data[10];
    uint32_t myInt = 0x11223344;
    Uint2Array(&myInt, data);
    if (data[0] == 0x11 &&
        data[1] == 0x22 &&
        data[2] == 0x33 &&
        data[3] == 0x44)
        log_info("TestUint2Array: pass");
    else
        log_info("TestUint2Array: fail");
    return 0;
}

uint8_t TestblOpenFlashFile()
{
    uint32_t segmentsCount;
    uint8_t addressAndSize[5][11];
    uint8_t checksum[5][8];
    blOpenFlashFile("test.HEX",&segmentsCount,addressAndSize,checksum);
    if (segmentsCount == 3)
        log_info("TestblOpenFlashFile TC1: pass");
    else
        log_info("TestblOpenFlashFile TC1: fail");
    if (addressAndSize[0][4] == 0x03 &&
        addressAndSize[0][10] == 0x40 &&
        addressAndSize[3][4] == 0x13 &&
        addressAndSize[3][10] == 0x70)
        log_info("TestblOpenFlashFile TC2: pass");
    else
        log_info("TestblOpenFlashFile TC2: fail");
    if(checksum[0][7]==0x86 &&
    checksum[3][7]==0xcd)
        log_info("TestblOpenFlashFile TC3: pass");
    else
        log_info("TestblOpenFlashFile TC3: fail");
    blOpenFlashFile("test.S19",&segmentsCount,addressAndSize,checksum);
    if (segmentsCount == 3)
        log_info("TestblOpenFlashFile TC4: pass");
    else
        log_info("TestblOpenFlashFile TC4: fail");
    if (addressAndSize[0][4] == 0x03 &&
        addressAndSize[0][10] == 0x80 &&
        addressAndSize[3][4] == 0x0f &&
        addressAndSize[3][10] == 0x10)
        log_info("TestblOpenFlashFile TC5: pass");
    else
        log_info("TestblOpenFlashFile TC5: fail");
    if(checksum[0][7]==0x69 &&
    checksum[3][7]==0x8a)
        log_info("TestblOpenFlashFile TC6: pass");
    else
        log_info("TestblOpenFlashFile TC6: fail");
    return 0;

}

uint8_t TestblBuffer()
{
    uint32_t segmentsCount;
    uint8_t addressAndSize[5][11];
    uint8_t checksum[5][8];
    uint8_t data[0xfff];
    uint32_t dataLength;
    blOpenFlashFile("test.HEX",&segmentsCount,addressAndSize,checksum);
    for(uint8_t i=0;i<=segmentsCount;i++)
    {
        uint32_t j=0;
        while (blBuffer(0xfff,data,&dataLength,i)==0)
        {
            if(j==0 && data[1]==0x01)
                log_info("TestblBuffer TC1: pass");
            else if(j==0)
                log_info("TestblBuffer TC1: fail");
            j++;
        }
    }
    if(data[1]==0x0b)
        log_info("TestblBuffer TC2: pass");
    else
        log_info("TestblBuffer TC2: fail");
    return 0;
}

int main(void)
{
    FileLoggerInit("testlog");

    TestSepcifyCRCParameters();
    TestCalculateCrcTable_CRC8();
    TestCalculateCrcTable_CRC16();
    TestCalculateCrcTable_CRC32();
    TestReflect8();
    TestReflect16();
    TestReflect32();
    TestCalculate_CRC8();
    TestCalculate_CRC16();
    TestCalculate_CRC32();
    TestAscCodedHex2Buffer();
    TestUint2Array();
    TestHandleHex();
    TestHandleSREC();
    TestblOpenFlashFile();
    TestblBuffer();
    return 0;
}
