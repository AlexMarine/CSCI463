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

#include "hex.h"

/**
 * @breif Gets string with 2 hex digits representing the 8 bits of the i argument
 * 
 * @param i Unsigned 8-bit integer which will be displayed as 2 hex digits
 * @return os.str String with exactly 2 hex digits representing the 8 bits of the i argument
 **********************************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
  return os.str();
}

/**
 * @brief Gets string with 8 hex digits representing the 32 bits of the i argument
 *
 * @param i Unsigned 32-bit integer which will be displayed as 8 hex digits
 * @return os.str String with exactly 8 hex digits representing 32 bits of the i agrument
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
 * @return String starting with 0x followed by the 8 hex digits from to_hex32(i)
 **********************************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
  return std::string("0x") + to_hex32(i);
}

/**
 * @brief Gets string beginning with 0x and the 5 hex digits representing 20 bits from i
 *
 * @param i Unsgined 32-bit integer which will be displayed as 0x, then 5 hex digits
 * @return String starting with 0x followed by the 5 hex digits representing 20 bits from i
 **********************************************************************************************/
std::string hex::to_hex0x20(uint32_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(5) << i;
  return std::string("0x") + os.str();
}

/**
 * @brief Gets string beginning with 0x and the 3 hex digits representing 12 bits from i
 *
 * @param i Unsigned 32-bit integer which will ne displayed as 0x, then 3 hex digits
 * @return String starting with 0x followed by the 3 hex digits representing 12 bits from i
 **********************************************************************************************/
std::string hex::to_hex0x12(uint32_t i)
{
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(3) << (i & 0x00000fff);
  return std::string("0x") + os.str();
}
