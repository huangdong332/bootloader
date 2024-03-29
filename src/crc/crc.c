/**
 * @file crc.c
 * @author Huang Dong (dohuang@borgwarner.com)
 * @brief This file contains functions to calculate CRC-* value of a file.
 * @version 0.1
 * @date 2023-05-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "crc.h"

//-------------------Handle CRC32 calculation-------------------------------
/**
 * @brief Local variables definition.
 * 
 */
uint32_t crcTable[256];
crcWidth width;
uint32_t polynomial = 0x04C11DB7;
uint32_t initialValue = 0x0;
uint32_t finalXORValue = 0x0;
uint8_t inputReflected=0, resultReflected=0;

/*
Read CRC algorithm specification form crcspec.
*/

/**
 * @brief Read CRC algorithm specification form crcspec file.
 * Local variables width, polynomial, initialValue, finalXORValue, inputReflected
 * and resultReflected will have the value defined in the crcspec file after this function call.
 * The crcspec file should be located in CANoe project root.
 * 
 * @return uint32_t 
 */
uint32_t appSepcifyCRCParameters()
{
  FILE * pFile;
  pFile=fopen("crcspec","r");
  LOG_INFO("Reading CRC parameters");
  char parameterName[30];
  uint32_t parameterValue;
  while (fscanf(pFile,"%s%8x",parameterName,&parameterValue)!=EOF)
  {
    if(strcmp(parameterName,"Polynomial")==0)
    {
      polynomial=parameterValue;
    }
    else if(strcmp(parameterName,"InitialValue")==0)
    {
      initialValue=parameterValue;
    }
    else if(strcmp(parameterName,"InputReflected")==0)
    {
      inputReflected=parameterValue;
    }
    else if(strcmp(parameterName,"ResultReflected")==0)
    {
      resultReflected=parameterValue;
    }
    else if(strcmp(parameterName,"FinalXORvalue")==0)
    {
      finalXORValue=parameterValue;
    }
    else if(strcmp(parameterName,"Width")==0)
    {
      if(parameterValue==0x8)
      width=CRC8;
      else if(parameterValue==0x16)
      width=CRC16;
      else if(parameterValue==0x32)
      width=CRC32;
    }
    else
    {
        LOG_ERROR("Invalid parameter: %s\n", parameterName);
    }
  }
  LOG_INFO("Width: %.8x",width);
  LOG_INFO("Polynomial: 0x%.8x",polynomial);
  LOG_INFO("Initial Value: 0x%.8x",initialValue);
  LOG_INFO("Input reflected: 0x%.8x",inputReflected);
  LOG_INFO("Result reflected: 0x%.8x",resultReflected);
  LOG_INFO("Final XOR value: 0x%.8x",finalXORValue);
  return 0;
}

/**
 * @brief Reflect an uint8 variable.
 * For example, an uint8 11001001(binary form) to uint8 10010011(binary form).
 * 
 * @param val An uint8 to be reflected.
 * @return uint8_t 
 */
uint8_t Reflect8(uint8_t val)
{
  uint8_t resVal = 0;

  for (int i = 0; i < 8; i++)
  {
    if ((val & (1 << i)) != 0)
    {
      resVal |= (uint8_t)(1 << (7 - i));
    }
  }

  return resVal;
}


/**
 * @brief Reflect an uint16 variable.
 * 
 * @param val An uint16 to be reflected.
 * @return uint16_t 
 */
uint16_t Reflect16(uint16_t val)
{
  uint16_t resVal = 0;

  for (int i = 0; i < 16; i++)
  {
    if ((val & (1 << i)) != 0)
    {
      resVal |= (uint16_t)(1 << (15 - i));
    }
  }

  return resVal;
}

/**
 * @brief Reflect an uint32 variable.
 * 
 * @param val An uint32 to be reflected.
 * @return uint32_t 
 */
uint32_t Reflect32(uint32_t val)
{
  uint32_t resVal = 0;

  for (int i = 0; i < 32; i++)
  {
    if ((val & (1 << i)) != 0)
    {
      resVal |= (uint32_t)(1 << (31 - i));
    }
  }

  return resVal;
}

/**
 * @brief Calculate CRC look up table for further CRC calculation.
 * Local variable polynomial will be used in this look up table calculation.
 * 
 * @return int32_t 
 */
int32_t CalculateCrcTable_CRC8()
{
    /* iterate over all byte values 0 - 255 */
    for (int divident = 0; divident < 256; divident++)
    {
        uint8_t currByte = (uint8_t)divident;
        /* calculate the CRC-8 value for current byte */
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if ((currByte & 0x80) != 0)
            {
                currByte <<= 1;
                currByte ^= polynomial;
            }
            else
            {
                currByte <<= 1;
            }
        }
        /* store CRC value in lookup table */
        crcTable[divident] = currByte;
    }
    return 0;
}

/**
 * @brief Calculate CRC look up table for further CRC calculation.
 * Local variable polynomial will be used in this look up table calculation.
 * 
 * @return int32_t 
 */
int32_t CalculateCrcTable_CRC16()
{
    for (int divident = 0; divident < 256; divident++) /* iterate over all possible input byte values 0 - 255 */
    {
        uint16_t curByte = (uint16_t)(divident << 8); /* move divident byte into MSB of 16Bit CRC */

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if ((curByte & 0x8000) != 0)
            {
                curByte <<= 1;
                curByte ^= polynomial;
            }
            else
            {
                curByte <<= 1;
            }
        }

        crcTable[divident] = curByte;
    }
    return 0;
}

/**
 * @brief Calculate CRC look up table for further CRC calculation.
 * Local variable polynomial will be used in this look up table calculation.
 * 
 * @return int32_t 
 */
uint8_t CalculateCrcTable_CRC32()
{
  for (int divident = 0; divident < 256; divident++) /* iterate over all possible input byte values 0 - 255 */
  {
    uint32_t curByte = (uint32_t)(divident << 24); /* move divident byte into MSB of 32Bit CRC */
    for (uint8_t bit = 0; bit < 8; bit++)
    {
      if ((curByte & 0x80000000) != 0)
      {
        curByte <<= 1;
        curByte ^= polynomial;
      }
      else
      {
        curByte <<= 1;
      }
    }

    crcTable[divident] = curByte;
  }
  return 0;
}

/**
 * @brief This is a warpper function.
 * Bit specific look up table calculation function will be called 
 * according to local variable width.
 * 
 * @return int32_t 
 */
int32_t CalculateCrcTable()
{
  switch (width)
  {
  case CRC8:
    CalculateCrcTable_CRC8();
    break;
  
  case CRC16:
    CalculateCrcTable_CRC16();
    break;
  
  case CRC32:
    CalculateCrcTable_CRC32();
    break;
  
  default:
    return 1;
    break;
  }
  return 0;
}

/**
 * @brief Calculate CRC8 of a file.
 * 
 * @param fileName Path to a file.
 * @return uint8_t Result CRC8.
 */
uint8_t Calculate_CRC8(const char* fileName)
{
    FILE * pFile;
    uint8_t tempChar;
    uint8_t crc = initialValue; /* CRC value is 32bit */

    LOG_INFO("Calculating checksum of tmep file: %s", fileName);

    pFile = fopen(fileName, "r");
    while (fscanf(pFile, "%2hhx", &tempChar) != EOF)
    {
        if(inputReflected==1)
        {
            tempChar=Reflect8(tempChar);
        }
        uint8_t pos = crc ^ tempChar;
        /* Shift out the MSB used for division per lookuptable and XOR with the remainder */
        crc = crcTable[pos];
        // crc=crcTable[((crc&0xff)^tempChar)]^(crc>>8);
    }
    if(resultReflected==1)
    {
        crc = Reflect8(crc);
    }
    crc ^= finalXORValue;
    LOG_INFO("Checksum of this temp file: 0x%.2X", crc);
    fclose(pFile);

    return crc;
}

/**
 * @brief Calculate CRC16 of a file.
 * 
 * @param fileName Path to a file.
 * @return uint16_t Result CRC16.
 */
uint16_t Calculate_CRC16(const char* fileName)
{
    FILE * pFile;
    uint16_t tempChar;
    uint16_t crc = initialValue; /* CRC value is 32bit */

    LOG_INFO("Calculating checksum of tmep file: %s", fileName);

    pFile = fopen(fileName, "r");
    while (fscanf(pFile, "%2hx", &tempChar) != EOF)
    {
        if(inputReflected==1)
        {
            tempChar=Reflect8(tempChar);
        }
        uint8_t pos = (uint8_t)((crc ^ (tempChar << 8)) >> 8);
        /* Shift out the MSB used for division per lookuptable and XOR with the remainder */
        crc = (uint32_t)((crc << 8) ^ crcTable[pos]);
        // crc=crcTable[((crc&0xff)^tempChar)]^(crc>>8);
    }
    if(resultReflected==1)
    {
        crc = Reflect16(crc);
    }
    crc ^= finalXORValue;
    LOG_INFO("Checksum of this temp file: 0x%.8X", crc);
    fclose(pFile);

    return crc;
}

/**
 * @brief Calculate CRC32 of a file.
 * 
 * @param fileName Path to a file.
 * @return uint32_t Result CRC32.
 */
uint32_t Calculate_CRC32(const char* fileName)
{
    FILE * pFile;
    uint32_t tempChar;
    uint32_t crc = initialValue; /* CRC value is 32bit */

    LOG_INFO("Calculating checksum of tmep file: %s", fileName);

    pFile = fopen(fileName, "r");
    while (fscanf(pFile, "%2x", &tempChar) != EOF)
    {
        if(inputReflected==1)
        {
            tempChar=Reflect8(tempChar);
        }
        uint8_t pos = (uint8_t)((crc ^ (tempChar << 24)) >> 24);
        /* Shift out the MSB used for division per lookuptable and XOR with the remainder */
        crc = (uint32_t)((crc << 8) ^ crcTable[pos]);
        // crc=crcTable[((crc&0xff)^tempChar)]^(crc>>8);
    }
    if(resultReflected==1)
    {
        crc = Reflect32(crc);
    }
    crc ^= finalXORValue;
    LOG_INFO("Checksum of this temp file: 0x%.8X", crc);
    fclose(pFile);

    return crc;
}

/**
 * @brief This is a warpper function.
 * Bit specific CRC calculation function will be called 
 * accroding to local variable width.
 * 
 * @param fileName 
 * @return uint32_t Result CRC value.
 */
uint32_t CalculateCrc(const char* fileName)
{
  uint32_t crc;
  switch (width)
  {
  case CRC8:
    crc=Calculate_CRC8(fileName);
    crc=crc<<24;
    break;

  case CRC16:
    crc=Calculate_CRC16(fileName);
    crc=crc<<16;
    break;

  case CRC32:
    crc=Calculate_CRC32(fileName);
    break;

  default:
    break;
  }

  return crc;
}