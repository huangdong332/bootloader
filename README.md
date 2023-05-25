<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.imgur.com/6wj0hh6.jpg" alt="Project logo"></a>
</p>

<h3 align="center">Bootloader Test CAPL DLL</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/huangdong332/bootloader)](https://github.com/JashonBourne/bootloader/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/huangdong332/bootloader)](https://github.com/kylelobo/The-Documentation-Compendium/pulls)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> A CAPL DLL supporting bootloader test.
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)
- [Built Using](#built_using)

## 🧐 About <a name = "about"></a>

This is a CAPL DLL project. This CAPL dll can be add to the CAPL environment to extend its functionalty.

There are two functions exported in ths dll.

blOpenFlashFile: Prase the Intel Hex file or Motorola SREC file and calculate checksum of each segment in it. Also extract the start address and size of each segment.

blBuffer: Extract data from specific segment in a HEX or SREC file and compose it to a complete UDS download service PDU.

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

MinGW and Make.

This CAPL dll is built with a MinGW gcc tools for 32bit Windows target. The 64bit CAPL dll can't be recognized by the CAPL browser any way. The compile process is managed by a Makefile.

```
I use msys64 to install MinGW and Make.
```

## 🎈 Usage <a name="usage"></a>

To use this CAPL dll in CAPL code, include it in your code first.
The path should be either the absolute or related to the CAPL code.

```
includes
{
  #pragma library("capl.dll")
}
```

After this, APIs in this dll are listed in CAPL Funtions window.

File crcspec

To specify CRC parameters in crcspec, below content should be 
added in file crcspec and this file should be located in CANoe project's root.

```
Width 32
Polynomial 04C11DB7
InitialValue FFFFFFFF
InputReflected 1
ResultReflected 1
FinalXORvalue FFFFFFFF
```

## ⛏️ Built Using <a name = "built_using"></a>

After change to this project's root directory, run follow command to build this CAPL dll.

```
make
```

To test this build, run

```
make test
```