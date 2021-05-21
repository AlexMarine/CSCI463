//***************************************************************************
//
//  Alex Marine (z1863905)
//  Assignment 4
//  CSCI 463-1
//  3/2/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#ifndef HEX_H
#define HEX_H

#include <set>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <unistd.h>

/**
 * Class for formatting numbers as hex strings for printing
 **********************************************************************************************/
class hex
{
public:
  static std::string to_hex8(uint8_t i);
  static std::string to_hex32(uint32_t i);
  static std::string to_hex0x32(uint32_t i);
  static std::string to_hex0x20(uint32_t i);
  static std::string to_hex0x12(uint32_t i);
};

#endif
