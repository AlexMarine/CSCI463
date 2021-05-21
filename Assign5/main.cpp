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

#include <iostream>
#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include "rv32i_hart.h"
#include "registerfile.h"
#include "cpu_single_hart.h"
#include <unistd.h>
#include <sstream>

using namespace std;

/**
 * @brief Usage message to explain correct command line use
 **********************************************************************************************/
static void usage()
{
  cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
  cerr << "    -d show disassembly before program execution" << endl;
  cerr << "    -i show instruction printing during execution" << endl;
  cerr << "    -l maximum number of instructions to exec" << endl;
  cerr << "    -m specify memory size (default = 0x100)" << endl;
  cerr << "    -r show register printing during execution" << endl;
  cerr << "    -z show a dump of the regs & memory after simulation" << endl;
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
 * @brief Get the option agruments and call the corresponding methods, then run the cpu
 *
 * @param argc Argument count
 * @param argv Argument values
 **********************************************************************************************/
int main(int argc, char **argv)
{
  uint32_t memory_limit = 0x100; //default memory size = 256
  int opt;
  uint64_t limit = 0; //the number of instructions to run

  //bool varibales so we can call other methods later in main
  bool dInsn = false;
  bool iInsn = false;
  bool rInsn = false;
  bool zInsn = false;

  while ((opt = getopt(argc, argv, "dirzm:l:")) != -1)
  {
    switch(opt)
    {
      //show a disassembly of the entire memory before program simulation begins
      case 'd':
        {
	  dInsn = true;
	}
        break;
      //show instruction printing during execution
      case 'i':
        {
	  iInsn = true;
        }
        break;
      //specifies the maximum limit of instructions to execute
      case 'l':
        {
	  limit = atoi(optarg);
        }
        break;
      //specifies the size of the simulated memory
      case 'm':
        {
          std::istringstream iss(optarg);
	  iss >> std::hex >> memory_limit;
        }
        break;
      //show a dump of the hart status before each instruction in simulated
      case 'r':
        {
	  rInsn = true;
        }
        break;
      //show a dump of the hart status and memory after the simulation has halted
      case 'z':
        {
	  zInsn = true;
        }
        break;
      default: /* '?' */
        usage();
    }
  }

  if (optind >= argc)
    usage(); //missing filename
 
  //initialize objects of memory
  memory mem(memory_limit);

  if (!mem.load_file(argv[optind]))
    usage();

  //initialize object of cpi_single_hart and call reset method
  cpu_single_hart cpu(mem);
  cpu.reset();

  //if optarg -d was called
  if (dInsn == true)
  {
    //call disassemble and reset the cpu
    disassemble(mem);
    cpu.reset();
  }

  //if optarg -i was called
  if (iInsn == true)
  { 
    //if optarg -r was called
    if (rInsn == true)
    {
      //set show registers to true
      cpu.set_show_registers(true);
    }
    //set show instructions to true
    cpu.set_show_instructions(true);
  }

  //call run with the number of instructions to run
  cpu.run(limit);
  
  //if optarg -z was called
  if (zInsn == true) 
  { 
    //call the cpu dump and the memory dump
    cpu.dump(); 
    mem.dump();
  }

  return 0;
}
