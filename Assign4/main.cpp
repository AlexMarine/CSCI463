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

#include <iostream>
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include <unistd.h>
#include <sstream>

using namespace std;

/**
 * @brief Usage message to explain correct command line use
 **********************************************************************************************/
static void usage()
{
  cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
  cerr << "    -m specify memory size (default = 0x100)" << endl;
  exit(1);
}

/**
 * @brief Disassembles the instruction that is stored in memory
 * 
 * @param mem The memory with which will be ouputted and decoded
 **********************************************************************************************/
static void disassemble(const memory &mem)
{
  uint32_t memoryAddress = 0;			//address of the piece of memory
  uint32_t insn = 0;				//instruction to output
  std::string renderedInsn;			//String of the decoded instruction

  //for each byte in memory (divide by 4 because each insn is 4 bytes()
  for (unsigned i = 0; i < mem.get_size() / 4; i++)
  {
    insn = mem.get32(memoryAddress);		 		//set the instruction to the one at the memory address
    renderedInsn = rv32i_decode::decode(memoryAddress, insn);	//decode the instrution located at the memory addres

    //output the memory address, instruction hex value, and the instruction mnemonic
    std::cout << hex::to_hex32(memoryAddress) << ": " << hex::to_hex32(insn) << "  " << 
	    renderedInsn << std::endl;

    memoryAddress += 4;	//increment the memory address to the next instruction
  }
}

/**
 * @breif Call the functions to load and disassemble binary RV32i instructions
 *
 * @param argc Argument count
 * @param argv Argument values
 **********************************************************************************************/
int main(int argc, char **argv)
{
  uint32_t memory_limit = 0x100; //default memory size = 256
  int opt;
  while ((opt = getopt(argc, argv, "m:")) != -1)
  {
    switch(opt)
    {
    case 'm':
      {
        std::istringstream iss(optarg);
	iss >> std::hex >> memory_limit;
      }
      break;
    default: /* '?' */
      usage();
    }
  }

  if (optind >= argc)
    usage(); //missing filename

  memory mem(memory_limit);

  if (!mem.load_file(argv[optind]))
    usage();

  disassemble(mem);
  mem.dump();

  return 0;
}
