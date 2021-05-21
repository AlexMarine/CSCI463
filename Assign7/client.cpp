//***************************************************************************
//
//  Alex Marine (z1863905)
//  Assignment 7
//  CSCI 463-1
//  4/23/2021
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment
//
//***************************************************************************

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>

/**
 * @brief Usage message to explain correct command line use
 **********************************************************************************************/
static void usage()
{
  std::cerr << "Usage: client [-s server-ip] server-port" << std::endl;
  std::cerr << "    -s server-ip: Specifies the server's IPv4 number in dotted-quad format (default 127.0.0.1)" << std::endl;
  std::cerr << "    server-port: The server port number which the client must connect" << std::endl;
  exit(1);
}

/**
 * @brief Same as write() but includes loop to complete any partial data
 *
 * @param fd File descriptor for the socket
 * @param buf The buffer that we want to write
 * @param len Length of the buffer
 *
 * @return -1 if there is an error, 0 if it worked correctly
 **********************************************************************************************/
static ssize_t safe_write(int fd, const char *buf, size_t len)
{
  while (len > 0)
  {
    ssize_t wlen = write(fd, buf, len);
    if (wlen == -1)
    {
      return -1; //write returned an unrecoverable error, error will be sent
    }
    len -= wlen; //reduce the remianing number of bytes to send
    buf += wlen; //advance the buffer pointer past the written data
  }
  return len; //if we get here then we sent the full requested length
}

/**
 * @brief Read data from the given socket file descriptor and print it until we reach EOF
 *
 * @param fd File descriptor for the socket
 *
 * @return -1 if there was an error, 0 if it worked
 **********************************************************************************************/
static int print_response(int fd)
{
  char buf[1024];
  int rval = 1; //prime the while loop

  while (rval > 0)
  {
    //Read in from the socket into the buffer
    if ((rval = read(fd, buf, sizeof(buf) - 1)) == -1)
    {
      perror("reading stream message");
      return -1; //let the caller know about the error
    }
    else if (rval > 0)
    {
      write(fileno(stdout), buf, rval);
    }
  }
  return 0;
}

/**
 * @brief Creates a socket that gets connected so a server with a user entered port and optional address number
 *
 * @param argc Argument count
 * @param argv Argument values
 *
 * @return 0
 **********************************************************************************************/
int main(int argc, char **argv)
{
  int opt;	//will hold the option arguements the user enetered
  int sock;	//FD for the socket connecting the server
  struct sockaddr_in server; //socket address for the server connection
  bool sCase = false; //Set to true if the user uses option argument -s

  while ((opt = getopt(argc, argv, "s:")) != -1)
  {
    switch (opt)
    {
      case 's':
      {
	//Set to true so we don't use the default address
        sCase = true;

        //converts the option argument into a IP4 address and write it into sin_addr for the server
        if (inet_pton(AF_INET, optarg, &server.sin_addr) <= 0)
        {
          std::cerr << optarg << ": invalid addresss/format" << std::endl;
          exit(2);
        }
      }
      break;
      default:
      {
        usage();
      }
    }
  }

  //Read raw bytes into a character array buffer from stdin
  char buf[300000]; //Will hold the data from stdin
  ssize_t len = read(fileno(stdin), buf, sizeof(buf));

  //create socket
  sock = socket(AF_INET, SOCK_STREAM, 0); //Use IPv4, use TCP protocol, and use default (which is TCP protocol)
  if (sock < 0)
  {
    //output error and close program
    perror("opening stream socket");
    exit(1);
  }

  //Connect socket using name specified by command line
  server.sin_family = AF_INET; //IPv4

  //If the option argument -s was not selected
  if (sCase == false)
  {
    //Use address 127.0.0.1 as the default
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0)
    {
      //This should never happen
      std::cerr << "127.0.0.1" << ": invalid addresss/format" << std::endl;
      exit(2);
    }
  }

  //Get the port number that the user entered
  server.sin_port = htons(atoi(argv[optind]));
  
  //Connect the file descriptor to the server
  if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0)
  {
    perror("connecting stream socket");
    exit(1);
  }
  if (safe_write(sock, buf, len) < 0)
  {
    perror("writing on stream socket");
  }

  //Tell the server that no more data will be sent
  //this will cause any subsequent read() calls to return EOF on the server side
  shutdown(sock, SHUT_WR);

  //call the print_response method
  print_response(sock);

  //close the socket
  close(sock);

  return 0;
}
