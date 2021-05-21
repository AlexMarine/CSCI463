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

#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

#include "rv32i_hart.h"

/**
 * Subclass of rv32i_hart that is used to represent a CPU with a single hart
 **********************************************************************************************/
class cpu_single_hart : public rv32i_hart
{
public:
  /**
   * @breif Pass the memory class instance to the constructor in the base class
   * 
   * @param mem Object of memory to pass to the rv32i_hart class
   *
   * @note Code given in assignment handout
   **********************************************************************************************/
  cpu_single_hart(memory &mem) : rv32i_hart(mem) { }

  void run(uint64_t exec_limit);
};

#endif
