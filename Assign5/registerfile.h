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

#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <iostream>
#include <cstdint>
#include <string>
#include <iomanip>
#include <vector>

/**
 * Class to store the state of the general-purpose registers of one RISC-V hart
 **********************************************************************************************/
class registerfile
{
public:
  void reset();
  void set(uint32_t r, int32_t val);
  int32_t get(uint32_t r) const;
  void dump(const std::string &hdr) const;
private:
  std::vector<int32_t> regs; //vector to hold all 32 registers
};

#endif
