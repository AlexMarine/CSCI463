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

#include "cpu_single_hart.h"

/**
 * @brief Will call tick() in a loop to execute instructions then output halt reason and number of instructions executed.
 *
 * @param exec_limit The maximum number of instructions that will be executed
 ***********************************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{
  //set r2 to the memory size
  regs.set(2, mem.get_size());

  //if the exec_limit paramater is 0, call tick() in a loop until the is_halted() returns true
  if (exec_limit == 0)
  {
    while (is_halted() == false)
    {
      tick();
    }
  }
  //else, the exec_limit is not 0, call tick() in a loop until is_halted() returns true
  //  or exec_limit number of instructions have been executed
  else
  {
    while (is_halted() == false && get_insn_counter() != exec_limit)
    {
      tick();
    }
  }

  //if hart becomes halted then print a message indicating why by using get_halt_reason()
  if (is_halted() == true)
  {
    std::cout << "Execution terminated. Reason: " << get_halt_reason() << std::endl;
  }

  //print the number of instructions that have been executed by using get_insn_counter()
  std::cout << std::dec << get_insn_counter() << " instructions executed" << std::endl;
}
