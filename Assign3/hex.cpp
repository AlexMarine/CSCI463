//***************************************************************************
//
//  Alex Marine (z1863905
//  Assignment 3
//  CSCI 463-1
//  1/29/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include "hex.h"

/**
 * @breif Returns string with 2 hex digits representing the 8 bits of the i argument
 *
 * @param i Unsigned 8-bit integer which will be displayed as 2 hex digits
 **********************************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
  return os.str();
}

/**
 * @brief Returns string with 8 hex digits representing the 32 bits of the i argument
 *
 * @param i Unsigned 32-bit integer which will be displayed as 8 hex digits
 **********************************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(8) << i;
  return os.str();
}

/**
 * @bried Calls to_hex32(uint32_t i) and concatenates 0x to the front of it
 *
 * @param i Unsigned 32-bit integer which will be displayed as 0x, then 8 hex digits
 **********************************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
  return std::string("0x") + to_hex32(i);
}
