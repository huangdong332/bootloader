#include "crc.h"

//-------------------Handle CRC32 calculation-------------------------------
uint32_t crcTable[256];
uint32_t polynomial = 0x04C11DB7;
uint32_t initialValue = 0x0;
uint32_t finalXORValue = 0x0;
uint8_t inputReflected=0, resultReflected=0;

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
    else
    {
        LOG_ERROR("Invalid parameter: %s\n", parameterName);
    }
  }
  LOG_INFO("Polynomial: 0x%.8x",polynomial);
  LOG_INFO("Initial Value: 0x%.8x",initialValue);
  LOG_INFO("Input reflected: 0x%.8x",inputReflected);
  LOG_INFO("Result reflected: 0x%.8x",resultReflected);
  LOG_INFO("Final XOR value: 0x%.8x",finalXORValue);
  return 0;
}

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


uint32_t Calculate_CRC32(const char* fileName)
{
    FILE * pFile;
    uint32_t tempChar;
    uint32_t crc = initialValue; /* CRC value is 32bit */

    LOG_INFO("Calculating checksum of tmep file: %s", fileName);

    pFile = fopen(fileName, "r");
    while (fscanf(pFile, "%2hhx", &tempChar) != EOF)
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
