/**
 * @file filepraser.c
 * @author Huang Dong (dohuang@borgwarner.com)
 * @brief This file contain functions to parse the Hex or SREC file.
 * @version 0.1
 * @date 2023-05-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "filepraser.h"

/**
 * @brief This function convert a string containning hex data to a char array.
 * For example, "120A3F" to {0x12, 0x0A, 0x3F}.
 * 
 * @param ascCodedHex A hex data string.
 * @param destinationBuffer A buffer to save the result hex data.
 * @return uint8_t 
 */
uint8_t AscCodedHex2Buffer(const char *ascCodedHex, uint8_t *destinationBuffer)
{
    uint32_t tempHex, i = 0;
    while (sscanf(ascCodedHex + 2 * i, "%2x", &tempHex) != EOF)
    {
        i++;
        *destinationBuffer = (uint8_t)tempHex;
        destinationBuffer++;
    }
    return 0;
}

/**
 * @brief This function converts an unsigned int to a unsigned char array.
 * For example, 120A3F to {0x12, 0x0A, 0x3F}.
 * 
 * @param targetUint A uint32 to be converted.
 * @param destinationArray A uint8 buffer to save the result data.
 * @return uint8_t 
 */
uint8_t Uint2Array(uint32_t *targetUint, uint8_t *destinationArray)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        destinationArray[i] = *((uint8_t *)targetUint + 3 - i);
    }
    return 0;
}

/**
 * @brief This function can parse a Hex file.
 * 
 * @param fileName A Hex file path.
 * @param segmentsCount The amount of blocks in a Hex file will be saved in this buffer.
 * @param addressAndSize The start address and size of each block will be saved in this buffer.
 * @param checksum The crc-* checksum of each block will be saved in this buffer.
 * @return uint8_t 
 */
uint8_t HandleHex(const char *fileName, uint32_t *segmentsCount, uint8_t addressAndSize[][8], uint8_t checksum[][4])
{
    FILE *pFile;
    FILE *tempFile = 0;
    char string[256];
    uint32_t extendedLinearAddress = 0x0;
    uint32_t accumulatedAddress = 0xffffffff, startAddress;
    char tempName[12] = "hex000"; // Temp file name.
    *segmentsCount = 0;
    // Start reading lines from SREC file until EOF.
    LOG_INFO("Open Intel HEX file: %s", fileName);
    pFile = fopen(fileName, "r");
    LOG_INFO("Reading lines from Intel Hex file");
    while (fscanf(pFile, "%s", string) != EOF)
    {
        uint32_t length, address, recordType;
        char tempString[256];
        sscanf(string, ":%2x%4x%2x%s", &length, &address, &recordType, tempString);
        switch (recordType)
        {
        case 0x00: // Data line
            address = extendedLinearAddress + address;
            if (address != accumulatedAddress)
            {
                // new segment begins. last segment ended.
                // accumulatedAddress is initialised as 0xffffffff
                // which means code here will not be executed when first segment begins.
                // Save current segment's start adress.
                if (accumulatedAddress != 0xffffffff)
                {
                lable1:
                    // When a segment ended.
                    // Close the open temp file for this segment.
                    // All data in this segment has been written to this file.
                    LOG_INFO("Close temp file: %s", tempName);
                    fclose(tempFile);
                    // Last segment's size equals to the difference of accumulatedAddress and startAddress of last segment.
                    uint32_t size = accumulatedAddress - startAddress;
                    // Save last segment's size to addressAndSize array.
                    // Code here saves a uint32_t to a uint8_t array
                    // with big endianness(Numerical data is saved as little endianness on Windows).
                    // Uint2Array(&size, addressAndSize[(*segmentsCount)] + 7);
                    Uint2Array(&size, addressAndSize[(*segmentsCount)] + 4);
                    //...
                    uint32_t crc = CalculateCrc(tempName); /* CRC value is 32bit */

                    // AscCodedHex2Buffer("31010202", checksum[(*segmentsCount)]);
                    // Uint2Array(&crc, checksum[(*segmentsCount)] + 4);
                    Uint2Array(&crc, checksum[(*segmentsCount)]);
                    // Print segment info to log file.
                    LOG_INFO("Address: 0x%.8x-%.8x Size: %.8x Checksum: %.8x",
                             startAddress, accumulatedAddress - 1, size, crc);

                    LOG_INFO("Segment %d completed", *segmentsCount);
                    
                    if (recordType == 0x01)
                        goto lable2;
                    // Increase segementsCount by 1 for every segment.
                    // Parentheses is necessary here because of high priority of ++.
                    (*segmentsCount)++;
                    sprintf(tempName+3,"%03.3d",*segmentsCount);
                    // tempName[5]++;
                }

                // When new segment begins.
                LOG_INFO("Segment %d started", *segmentsCount);
                // Save current segment's start address to addressAndSize array.
                // Code here saves a uint32_t to a uint8_t array
                // with big endianness(Numerical data is saved as little endianness on Windows).
                startAddress = address;
                // Reset accumulated address as start address.
                accumulatedAddress = address;
                // AscCodedHex2Buffer("340044", addressAndSize[*segmentsCount]);
                // Uint2Array(&address, addressAndSize[*segmentsCount] + 3);
                Uint2Array(&address, addressAndSize[*segmentsCount]);
                //...
                // Open a new temp file.
                LOG_INFO("Open temp file: %s", tempName);
                tempFile = fopen(tempName, "w");
            }
            // Write the data field of a data line to temp file.
            fprintf(tempFile, "%.*s", length * 2, tempString);
            // Increase accumulatedAddress by (length -5)(Address and checksum in data line consume 5 bytes).
            accumulatedAddress = address + length;
            break;
        case 0x01: // End of File
            goto lable1;
        lable2:
            break;
        case 0x02: // Extended Segment Address
            sscanf(tempString, "%4x", &address);
            extendedLinearAddress = address * 16;
            LOG_INFO("extendedSegmentAddress :%x", extendedLinearAddress);
            break;
        case 0x03: // Start Segment Address, Not used here.
            break;
        case 0x04: // Extended linear address
            sscanf(tempString, "%4x", &address);
            extendedLinearAddress = address * 0x10000;
            LOG_INFO("extendedLinearAddress :%x", extendedLinearAddress);
            break;
        case 0x05: // Start linear address
            break;
        default:
            break;
        }
    }
    LOG_INFO("Close Intel HEX file: %s", fileName);
    fclose(pFile);
    return 0;
}

/**
 * @brief This function can parse a SREC file.
 * 
 * @param fileName A SREC file path.
 * @param segmentsCount The amount of blocks in a Hex file will be saved in this buffer.
 * @param addressAndSize The start address and size of each block will be saved in this buffer.
 * @param checksum The crc-* checksum of each block will be saved in this buffer.
 * @return uint8_t 
 */
uint8_t HandleSREC(const char *fileName, uint32_t *segmentsCount, uint8_t addressAndSize[][8], uint8_t checksum[][4])
{
    FILE *tempFile = 0, *pFile;
    char string[256];
    uint32_t accumulatedAddress = 0xffffffff, startAddress;
    char tempName[12] = "hex000"; // Temp file name.
    *segmentsCount = 0;
    // Start reading lines from SREC file until EOF.
    LOG_INFO("Open SREC file: %s", fileName);
    pFile = fopen(fileName, "r");
    LOG_INFO("Reading lines from SREC file %s",fileName);
    while (fscanf(pFile, "%s", string) != EOF)
    {
        uint32_t address, length, recordType; // Save length in the data line.
        char tempString[255];                 // String buffer to save data read from a line.
        // Get address, length and data field from a data line.
        sscanf(string, "S%1x%2x", &recordType, &length);
        switch (recordType)
        {
        case 0x00:
            LOG_INFO("First line: %s", string);
            break;
        case 0x01:
        case 0x02:
        case 0x03:
            if (recordType == 0x1)
            {
                sscanf(string, "S%*1x%*2x%4x%s", &address, tempString);
            }
            else if (recordType == 0x2)
            {
                sscanf(string, "S%*1x%*2x%6x%s", &address, tempString);
            }
            else if (recordType == 0x3)
            {
                sscanf(string, "S%*1x%*2x%8x%s", &address, tempString);
            }
            if (address != accumulatedAddress)
            {
                // new segment begins. last segment ended.
                // accumulatedAddress is initialised as 0xffffffff
                // which means code here will not be executed when first segment begins.
                // Save current segment's start adress.
                if (accumulatedAddress != 0xffffffff)
                {
                lable1:
                    // When a segment ended.
                    // Close the open temp file for this segment.
                    // All data in this segment has been written to this file.
                    LOG_INFO("Close temp file: %s", tempName);
                    fclose(tempFile);
                    // Last segment's size equals to the difference of accumulatedAddress and startAddress of last segment.
                    uint32_t size = accumulatedAddress - startAddress;
                    // Save last segment's size to addressAndSize array.
                    // Code here saves a uint32_t to a uint8_t array
                    // with big endianness(Numerical data is saved as little endianness on Windows).
                    // Uint2Array(&size, addressAndSize[(*segmentsCount)] + 7);
                    Uint2Array(&size, addressAndSize[(*segmentsCount)] + 4);
                    //...
                    uint32_t crc = CalculateCrc(tempName); /* CRC value is 32bit */

                    // AscCodedHex2Buffer("31010202", checksum[(*segmentsCount)]);
                    // Uint2Array(&crc, checksum[(*segmentsCount)] + 4);
                    Uint2Array(&crc, checksum[(*segmentsCount)]);
                    // Print segment info to log file.
                    LOG_INFO("Address: 0x%.8x-%.8x Size: %.8x Checksum: %.8x",
                             startAddress, accumulatedAddress - 1, size, crc);

                    LOG_INFO("Segment %d completed", *segmentsCount);

                    if (recordType == 0x07 ||
                        recordType == 0x08 ||
                        recordType == 0x09)
                        goto lable2;
                    // Increase segementsCount by 1 for every segment.
                    // Parentheses is necessary here because of high priority of ++.
                    (*segmentsCount)++;
                    sprintf(tempName+3,"%03.3d",*segmentsCount);
                    // tempName[5]++;
                }

                // When new segment begins.
                LOG_INFO("Segment %d started", *segmentsCount);
                // Save current segment's start address to addressAndSize array.
                // Code here saves a uint32_t to a uint8_t array
                // with big endianness(Numerical data is saved as little endianness on Windows).
                startAddress = address;
                // Reset accumulated address as start address.
                accumulatedAddress = address;
                // AscCodedHex2Buffer("340044", addressAndSize[*segmentsCount]);
                // Uint2Array(&address, addressAndSize[*segmentsCount] + 3);
                Uint2Array(&address, addressAndSize[*segmentsCount]);
                //...
                // Open a new temp file.
                LOG_INFO("Open temp file: %s", tempName);
                tempFile = fopen(tempName, "w");
            }
            // Write the data field of a data line to temp file.
            fprintf(tempFile, "%.*s", length * 2 - 2 * (recordType + 2), tempString);
            // Increase accumulatedAddress by (length -5)(Address and checksum in data line consume 5 bytes).
            accumulatedAddress = address + length - (recordType + 2);
            break;
        case 0x07:
        case 0x08:
        case 0x09:
            goto lable1;
        lable2:
            break;
        default:
            break;
        }
    }
    LOG_INFO("Close SREC file: %s", fileName);
    fclose(pFile);
    return 0;
}
