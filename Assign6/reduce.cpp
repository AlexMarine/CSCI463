//***************************************************************************
//
//  Alex Marine (z1863905)
//  Assignment 6
//  CSCI 463-1
//  4/9/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <unistd.h>

//Global variable definitions
constexpr int rows = 1000;	///< the number of rows in the work matrix
constexpr int cols = 100;	///< the number of cols in the work matrix

std::mutex stdout_lock;		///< for serializing access to stdout

std::mutex counter_lock;	///< for dynamic balancing only
volatile int counter = rows;	///< for dynamic balancing only

std::vector<int> tcount;	///< count of rows summed for each thread
std::vector<uint64_t> sum;	///< the calculated sum from each thread

int work[rows][cols];		///< the matrix to be summed

/**
 * @brief Usage message to explain correct command line use
 **********************************************************************************************/
static void usage()
{
  std::cerr << "Usage: reduce [-d] [-t num]" << std::endl;
  std::cerr << "    -d use dynamic load balancing" << std::endl;
  std::cerr << "    -t specifies the number of threads to use" << std::endl;
  exit(1);
}

/**
 * @brief Uses static load balancing to determine which rows will be processed by each thread
 *
 * @param tid Thread ID
 * @param num_threads Number of threads
 **********************************************************************************************/
void sum_static(int tid, int num_threads)
{
  //Output and specify which thread is starting by its thread ID
  stdout_lock.lock();
  std::cout << "Thread " << tid << " starting" << std::endl;
  stdout_lock.unlock();

  int row = 0;		//the row that will be summed through
  int count = 0;	//counter
  bool done = false;	//tells us when to stop summing through rows
  
  while (!done)
  {
    //calculate the next row to sum then increment the counter
    row = tid + (num_threads * count);
    ++count;

    //if the row is out of bonds, don't sum it
    if ( row >= rows) { done = true; }

    //if the row is elegible to be summed through
    if (!done)
    {
      //for every column in the row, add the value to the total
      uint64_t total = 0;
      for (int i = 0; i < cols; ++i)
      {
        total += work[row][i];
      }

      //count the sum of each row that this thread has done
      sum[tid] += total;

      //count the number of work units that this thread has done
      ++tcount[tid];
    }
  }

  //Output the thread ID, the numbers of rows it processed, and the total sum of each row it processed
  stdout_lock.lock();
  std::cout << "Thread " << tid << " ending tcount=" << tcount[tid] << " sum=" << sum[tid] << std::endl;
  stdout_lock.unlock();
}

/**
 * @brief Uses dynamic load balancing to determine which rows will be processed by each thread
 *
 * @param tid Thread ID
 **********************************************************************************************/
void sum_dynamic(int tid)
{
  //Output and specify which thread is starting by its thread ID
  stdout_lock.lock();
  std::cout << "Thread " << tid << " starting" << std::endl;
  stdout_lock.unlock();

  bool done  = false; //tells us when to stop summing through rows

  while (!done)
  {
    int count_copy; //hold a copy of the counter to use later

    counter_lock.lock();
    {
      //If the counter is greater than 0, decrement it
      if (counter > 0) { --counter; }
      //Else, don't sum through the row
      else { done = true; }

      count_copy = counter;	//grab a copy that we know is consistent with done flag
    }
    counter_lock.unlock();

    //If the row is elegible to be summed through
    if (!done)
    {
      //do work using the value of count_copy to determine what we do
      (void)count_copy;	//remove compiler wanring

      //for every column in a row, add the value to the total
      uint64_t total = 0;
      for (int i = 0; i < cols; ++i)
      {
        total += work[count_copy][i];
      }

      //count the sum of each row that this thread has done
      sum[tid] += total;

      //count the number of work units that this thread has done
      ++tcount[tid];
    }
  }

  //Output the thread ID, the number of rows it processed, and the total sum of each row it processed
  stdout_lock.lock();
  std::cout << "Thread " << tid << " ending tcount=" << tcount[tid] << " sum=" << sum[tid] << std::endl;
  stdout_lock.unlock();
}

/**
 * @brief Accepts command-line parameters to either use static or dynamic loading, and set the number of threads
 *
 * @param argc Argument count
 * @param argv Argument values
 *
 * @return 0
 **********************************************************************************************/
int main(int argc, char **argv)
{
  int opt;				//will hold the option arguements the user enetered
  unsigned int num_threads = 2; 	//number of threads to be used. Defaults to 2
  bool dCase = false;   		//If the user uses option argument d, set to true
  std::vector<std::thread*> threads;	//will hold the threads we use

  while ((opt = getopt(argc, argv, "dt:")) != -1)
  {
    switch (opt)
    {
      //Use dynamic load balancing
      case 'd':
      {
        dCase = true;
      }
      break;
      //Specifies the number of threads to use
      case 't':
      {
	//store the number in a variable
        num_threads = atoi(optarg);

	//if the number entered is greater than or equal to the number of threads the system can use
	if (num_threads >= std::thread::hardware_concurrency())
	{
          //use the number of threads the system can use
          num_threads = std::thread::hardware_concurrency();
	}
      }
      break;
      default:
      {
        usage();
      }
    }
  }

  //Initialize the data in the global work matrix using the rand() function
  srand(0x1234); //seed the random number generator
  for (int i = 0; i < cols; i++)
  {
    for (int j = 0; j < rows; j++)
    {
      work[j][i] = rand();
    }
  }

  //Output the number of cores in the system
  std::cout << std::thread::hardware_concurrency() << " concurrent threads supported." << std::endl;

  //Resize the global vectors to size of number of threads and initialize to 0
  tcount.resize(num_threads, 0);
  sum.resize(num_threads, 0);
  
  //If optarg -d was input, use dynamic load balancing
  if (dCase)
  {
    //add the threads to the vector and call sum_dynamic
    for (unsigned int i = 0; i < num_threads; ++i)
    {
      threads.push_back(new std::thread(sum_dynamic, i));
    }
  }
  //Else, use static load balancing
  else
  {
    //Sdd the threads to the vector and call sum_static
    for (unsigned int i = 0; i < num_threads; ++i) 
    { 
      threads.push_back(new std::thread(sum_static, i, num_threads));
    }
  }

  //Join the threads then delete them while calculating the total work all threads have done
  int total_work = 0;
  for (unsigned int i =0; i < num_threads; ++i)
  {
    threads.at(i)->join();
    delete threads.at(i);
    total_work += tcount.at(i);
  }

  //Get the gross sum of the threads
  uint64_t gross_sum = 0;
  for (unsigned int i = 0; i < num_threads; ++i)
  { 
    gross_sum += sum[i];
  }

  //Output the total_work and gross_sum then exit the program
  std::cout << "main() exiting, total_work=" << total_work << " gross_sum=" << gross_sum << std::endl;
  return 0;
}
