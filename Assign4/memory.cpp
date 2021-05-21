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

#include "memory.h"
#include "hex.h"

/**
 * @brief Allocate siz bytes in the mem vector and initialize every byte to 0xa5
 *
 * @param siz The number of bytes allocated to vector, mem
 **********************************************************************************************/
memory::memory(uint32_t siz)
{
  //round the length up, mod-16
  siz = (siz + 15) & 0xfffffff0;

  //set 'siz' bytes in mem to 0xa5
  for (uint32_t i = 0; i < siz; i++)
  {
    mem.push_back(0xa5);
  }
}

/**
 * @breif Clear out memory vector
 **********************************************************************************************/
memory::~memory()
{
  mem.clear();
}

/**
 * @breif Checks if the address is in the simulated memory
 *
 * @param i Address to be checked if it is in memory
 *
 * @return true if address is in memory, false if it is not
 **********************************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
  //if the byte is not in memoru
  if(i > mem.size())
  {
    //print an error message and return true
    std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
    return true;
  }
  //else, the byte is in memory and return false
  else
  {
    return false;
  }
}  

/**
 * @brief Gets the rounded up number of bytes in the simulated memory
 * 
 * @return The size of the memory vector
 **********************************************************************************************/
uint32_t memory::get_size() const
{
  return mem.size();
}

/**
 * @brief Call check_illegal(uint32_t) to see if addr is in the vector
 *
 * @param addr Address that will be checked if it is in memory
 *
 * @return addr if it is in mem, 0 if it is not
 **********************************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
  //if the byte is in memory return it
  if (check_illegal(addr) == false)
  {
   //return what is stored in memory at the address
    return mem[addr];
  }
  //else the byte is not in memory and return 0
  else
  {
    return 0;
  }
}

/**
 * @breif Calls get8 twice and formats addr in little endian order
 *
 * @param addr Address that will be sent to get8 twice then put into little endian
 *
 * @return The little endian order of addr in 16-bits
 **********************************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
  //variable definitions for left and right side for little endian order of a 16-bit value
  uint16_t left = get8(addr);
  uint16_t right = get8(addr + 1);

  //combine the left and right for the full little endian order and return it
  uint32_t littleE = (right << 8) | left;
  return littleE;
}

/**
 * @breif Calls get16 twice and formats addr in little endian order
 *
 * @param addr Address that will be sent to get16 twice then put into little endian
 *
 * @return The little endian order of addr in 32-bits
 **********************************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
  //variable deifinitions for the left and right side for little endian order of a 32-bit value
  uint32_t left = get16(addr);
  uint32_t right = get16(addr + 2);

  //combine the left and right for little endian order and return it
  uint32_t littleE = (right << 16) | left;
  return littleE;
}

/**
 * @breif Calls get8 and returns a sign-extended value of addr as a 32-bit signed integer
 *
 * @param addr Address that will be sent to get8 and be sign-extended to 32-bit
 *
 * @return Addr as a 32-bit signed integer 
 **********************************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
  return (int32_t)get8(addr);
}

/**
 * @breif Calls get16 and returns a sign-extended value of addr as a 16-bit signed integer
 *
 * @param addr Address that will be sent to get16 and be sign-extended to 16-bit
 *
 * @return Addr as a 16-bit signed integer
 **********************************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
  return (int32_t)get16(addr);
}

/**
 * @breif Calls get32 and returns the value as a 32-bit signed integer
 *
 * @param addr Address that will be sent to get32
 *
 * @return Addr as a 32-bit signed integer
 **********************************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
  return get32(addr);
}

/**
 * @breif Calls check_illegel to see if addr is valid. If it is, set the byte in memory to val
 *
 * @param addr Address that will have a value stored in it, if it is legal
 * @param val Value that will be stored at the address in memory, if it is legal
 **********************************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{  
  if (check_illegal(addr) == false)
  {
    mem[addr] = val;
  }
}

/**
 * @brief Calls set8 twice to store val in little endian order in memory at the address
 *
 * @param addr Address that will have a value stored in it, if it is legal
 * @param val Value that will be stored at the address in memory, if it is legal
 **********************************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
  //variable definition for the left and right side of the little endian order of a 16-bit value
  uint8_t left = (val & 0xff00) >> 8;
  uint8_t right = (val & 0x00ff);

  //call set 8 twice to store the 16 bit value at the address
  set8(addr + 1, left);
  set8(addr, right);
}

/**
 * @brief Calls get16 twice to store val in little endian order in memory at the address
 *
 * @param addr Address that will have a value stored in it, if it is legal
 * @param val Value that will be stored at the address in memory, if it legal
 **********************************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
  //variable definition for the left and right side of the little endian order of a 32-bit value
  uint16_t left = (val & 0xffff0000) >> 16;
  uint16_t right = (val & 0x0000ffff);

  //call set 16 twice to store the 32 bit value at the address
  set16(addr + 2, left);
  set16(addr, right);
}

/**
 * @breif Dump the contents of the simulated memory in hex along with ASCII
 **********************************************************************************************/
void memory::dump() const
{
  //array of ascii characters to print content in memory and set it to ascii zero
  char arr[17];
  arr[16] = '\0';

  //for each byte in memory, format the dump
  for (unsigned i = 0; i < mem.size(); i++)
  {
    //store a '.' if the byte does not have a valid printable value
    uint8_t ch = get8(i);
    ch = isprint(ch) ? ch : '.';

    //store the ascii values in an array to print later
    arr[i % 16] = ch;

    //if at the start of a new value, print the address in hex
    if ((i % 16) == 0) { std::cout << hex::to_hex32(i) << ": "; }

    //output the value two hex bytes at a time, with spaces in between them
    std::cout << hex::to_hex8(get8(i)) << " ";
    
    //output an extra space in the middle of the value
    if ((i % 16) == 7) { std::cout << " "; }

    //output the ascii value of the value in memory
    if ((i % 16) == 15) { std::cout << "*" << arr << "*" << std::endl; }
  }
}

/**
 * @breif Open the file in binary mode and write the content of the file to memory
 *
 * @param fname Name of the file to be read
 *
 * @return False if the file can't be opened or it exceeds program size.
 * 		True if the file can be opened, read, and written to memory
 **********************************************************************************************/
bool memory::load_file(const std::string &fname)
{
  //open the file in binary mode
  std::ifstream infile(fname, std::ios::in|std::ios::binary);
  uint8_t i;

  //read the whitespaces in the file
  infile >> std::noskipws;

  //if the file is not open, print an error message and return false
  if (!infile.is_open())
  {
    std::cout << std::endl;
    std::cerr << "Can't open file '" << fname << "' for reading." << std::endl;
    return false;
  }
  else
  {
    //for each byte in the file, continue
    for (uint32_t addr = 0; infile >> i; addr++)
    {
      //if the address does not fit in memory, print an error message, close file, and return false
      if (check_illegal(addr))
      {
        std::cerr << "Program too big." << std::endl;
	infile.close();
	return false;
      }
      //else, set the byte in memory to the byte being read in
      else
      {
        mem[addr] = i;
      }
    }
  }
  
  infile.close();
  return true;
}
