//***************************************************************************
//
//  Alex Marine (z1863905
//  Assignment 2
//  CSCI 463-1
//  1/29/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include <iostream>
#include <iomanip>
#include <cstdint>

/**
 * @brief Prints the parts that make up the IEEE-754 representation of the inputted value.
 *
 * Takes the inputted 32-bit unsigned hex integer and outputs it in binary. Then, outputs 
 *   the sign bit of the ineteger, as well as the exponent and signifiacand as a 32-bit hex 
 *   value. Finally, outputs the full value of the number in binary.
 *
 * @param x 32-bit hex value that will have its various IEEE-754 components found and outputted
 **********************************************************************************************/
void printBinFloat(uint32_t x)
{
  uint mask = 0x80000000; ///< Used to select the needed bits of x

  //output the hex integer
  std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << x << " = ";

  //for loop to output the hex integer in binary
  for (int i = 1; i < 33; i++)
  {
    std::cout << (x & mask ? '1':'0');
    mask >>= 1;

    //print a space every 4 bits, excluding the last bit
    if (!(i % 4) && !( i == 32))
    {
      std::cout << " ";
    }
  }

  int sign = x >> 31; ///< Holds the sign but of x
  std::cout << std::endl << "sign: " << sign << std::endl;

  mask = 0x8000000; //reset the mask
  mask <<= 3; //shift the mask for finding the exponent

  int expSum = 128; ///< Subtract expPower from expSum to find the final value
  uint expPower = 128; ///< Keeps track of which place we are in for the exponent

  //for loop to find value of the exponent
  for (int i = 1; i < 9; i++)
  { 
    //if the bit in x is not equal to 1
    if (!(x & mask))
    {
      expSum = expSum - expPower;
    }

    expPower = expPower / 2;
    mask >>= 1;
  }

  //output the exponent in hex and then decimal
  std::cout << " exp: 0x" << std::hex << std::setw(8) << std::setfill('0')
	  << expSum << " (" << std::dec << expSum << ")" << std::endl;

  mask = 0x8000000; //reset the mask
  mask >>= 5; //set the mask for finding the significand

  uint sigSum = 0; ///< Add expPower to sigSum to find the final value
  uint sigPower = 4194304; ///< Keeps track of which place we are in for the significand
  int counter = 0;

  //for loop to find the value of the significand
  for (int i = 1; i < 24; i++)
  {
    //if the bit in x is equal to 1
    if (x & mask)
    {
      sigSum = sigSum + sigPower;
    }

    sigPower = sigPower / 2;
    mask >>= 1;
  }

  //output the significand in hex
  std::cout << " sig: 0x" << std::hex << std::setw(8)
	  << std::setfill('0') << sigSum << std::endl;

  //if integer equals postive infinity
  if (sign == 0 && expSum == 128 && sigSum == 0)
  {
    std::cout << "+inf";
  }
  //if integer equals negative infinity
  else if (sign == 1 && expSum == 128 && sigSum == 0)
  {
    std::cout << "-inf";
  }
  //if integer equals postive zero
  else if (sign == 0 && expSum == -127 && sigSum == 0)
  {
    std::cout << "+0";
  }
  //if integer equals negative zero
  else if (sign == 1 && expSum == -127 && sigSum == 0)
  {
    std::cout << "-0";
  }
  //else, the integer is not a special case
  else
  {
    mask = 0x8000000; //reset the mask
    mask >>= 5; //set the mask for finding the significand
    
    //if the integer is postive
    if (sign == 0)
    {
      std::cout << "+";
    }
    //else, the integer is negative
    else
    {
      std::cout << "-";
    }

    if (expSum < 0)
    {
      //will always start with this when expSum < 0
      std::cout << "0.";

      //get absolute value of expSum and subtract 1
      expSum = expSum - expSum - expSum - 1;

      //print zeros based on number of expSum
      for (int i = 0; i < expSum; i++)
      {
        std::cout << "0";
      }

      //implied 1 on left of significand
      std::cout << "1";

      //print the significand
      for (uint i = 1; i < 24; i++)
      {
        std::cout << (x & mask ? '1':'0');
        mask >>= 1;
      }
    }
    else if (expSum > 0)
    {
      //implied 1 on left of significand
      std::cout<< "1";

      //print the significand
      for (int i = 0; i < expSum; i++)
      {
        std::cout << (x & mask ? '1':'0');
	mask >>= 1;
	counter++;
      }

      std::cout << ".";

      //print the rest of the significand if needed
      for (int i = 0; i < 24 - counter; i++)
      {
        std::cout << (x & mask ? '1':'0');
	mask >>= 1;
      }

      //print a zero after decimal place when expSum is too big
      if (expSum > 23)
      {
        std::cout << "0";
      }
    }
    else //if expSum = 0
    {
      //will always start with this when expSum = 0
      std::cout << "1.";

      //print out the significand in binary
      for (int i = 1; i < 24; i++)
      {
        std::cout << (x & mask ? '1':'0');
        mask >>= 1;
      }
    }
  }
}

/**
 * @brief Gets user inputted unsigned 32 bit hex integer and calls printBinFloat(x)
 * @return 0
 **********************************************************************************/
int main()
{
  uint32_t x; ///< 32-bit hex value that will be passed to printBinFloat

  //continue while there is values to be read in
  while (std::cin >> std::hex >> x)
  {
    printBinFloat(x);
    std::cout << std::endl;
  }

  if (!std::cin.eof())
  {
    std::cerr << "WARNING: Terminating due to input stream error" << std::endl;
  }

  return 0;
}
