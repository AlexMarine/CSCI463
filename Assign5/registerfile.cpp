//***************************************************************************
//
//  Alex Marine (z1863905)
//  Assignment 5
//  CSCI 463-1
//  3/23/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include "registerfile.h"
#include "hex.h"

/**
 * @breif Initialize register x0 to zero, and all other registers to 0xf0f0f0f0
 **********************************************************************************************/
void registerfile::reset()
{
  //set 32 registers in the vector to 0xf0f0f0f0
  for (int32_t i = 0; i < 32; i++) 
  { 
    regs.push_back(0xf0f0f0f0);
  }

  //set register 0 to 0x00000000
  regs[0] = 0x00000000;
}

/**
 * @breif Assign register r the given val. If r is zero then do nothing
 * 
 * @param r Given register
 * @param val Value to be stored in the given register
 **********************************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
  //if the register is not x0, assign the value
  if (r != 0) { regs[r] = val; }
}

/**
* @breif Return the value of register r. If r is zero then return zero
* 
* @param r Given register
*
* @return The value of register r, or 0 if r is x0
**********************************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
  //if r is not 0, return the register r
  if (r != 0) { return regs[r]; }
  //else, r is 0, so return 0
  else { return 0; }
}

/**
 * @breif Dump the contents stored inthe registers
 *
 * @param hdr A string that must be printed at the beginnng of the output lines
 **********************************************************************************************/
void registerfile::dump(const std::string &hdr) const
{
  //output the header for the line and register 0
  std::cout << hdr << std::right << std::setw(3) << "x0";
  //output registers 0-7
  for (int32_t i = 0; i <= 7; i++) 
  { 
    std::cout << " " << hex::to_hex32(regs[i]);

    //after 4 registers, output an extra space
    if (i == 3) { std::cout << " "; }
  }
  std::cout << std::endl;

  //output the header for the line and register 0
  std::cout << hdr << std::right << std::setw(3) << "x8";
  //output registers 8-15
  for (int32_t i = 8; i <= 15; i++) 
  { 
    std::cout << " " << hex::to_hex32(regs[i]);
    if (i == 11) { std::cout << " "; }
  }
  std::cout << std::endl;

  //output the header for the line and register 0
  std::cout << hdr << std::right << std::setw(3) << "x16";
  //output registers 16-23
  for (int32_t i = 16; i <= 23; i++) 
  { 
    std::cout << " " << hex::to_hex32(regs[i]);
    if (i == 19) { std::cout << " "; }
  }
  std::cout << std::endl;

  //output the header for the line and register 0
  std::cout << hdr << std::right << std::setw(3) << "x24";
  //output registers 24-31
  for (int32_t i = 24; i <= 31; i++) 
  { 
    std::cout << " " << hex::to_hex32(regs[i]);
    if (i == 27) { std::cout << " "; }
  }
}
