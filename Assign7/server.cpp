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
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <signal.h>

/**
 * @brief Usage message to explain correct command line use
 **********************************************************************************************/
static void usage()
{
  std::cerr << "Usage: client listener-port" << std::endl;
  std::cerr << "    listener-port: The port number to which the server must listen" << std::endl;
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
 * @brief Creates a socket with a user entered port that will accept connections from a client and send back return data
 *
 * @param argc Argument count
 * @param argv Argument values
 *
 * @return 0
 **********************************************************************************************/
int main(int argc, char **argv)
{
  int opt;	//will hold the option arguements the user enetered
  int sock;	//FD for the socket
  socklen_t length;	//The size of the server
  struct sockaddr_in server; //Socket address for the client connection
  int msgsock;	//FD for the socket that will contain the return message to the client
  char buf[1024];	//Will hold the characters to send to the client
  bool lCase = false; 	//set to true if option argument -l is used
  uint32_t totalLen = 0;	//Total byte count sent from the client
  uint16_t checkSum = 0;	//The sum of the bytes sent from the client
  ssize_t len = 0;	//The number of bytes returned from read

  while ((opt = getopt(argc, argv, "l:")) != -1)
  {
    switch (opt)
    {
      case 'l':
      {
	//Set to true so we don't use the default port
        lCase = true;

        //Set the listener port to what the user entered
        server.sin_port = htons(atoi(optarg));
      }
      break;
      default:
      {
        usage();
      }
    }
  }

  //Ignore all broken pipes
  signal(SIGPIPE, SIG_IGN);

  //Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("opening stream socket");
    exit(1);
  }

  //Name socket using wildcards
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY; //sets address
	  
  //If the user did not use the -l option argument
  if (lCase == false)
  {
    //Set the listener port to an ephemeral port number
    server.sin_port = 0;
  }

  //bind a name to the socket so we know the address and port
  if (bind(sock, (sockaddr*)&server, sizeof(server)))
  {
    perror("binding stream socket");
    exit(1);
  }

  //Find out assigned port number and print it out
  length = sizeof(server);
  if (getsockname(sock, (sockaddr*)&server, &length))
  {
    perror("getting socket name");
    exit(1);
  }
  //Output the port number of the socket
  std::cout << "Socket has port #" << ntohs(server.sin_port) << std::endl;

  //Start accepting connections
  listen(sock, 5);
  do
  {
    struct sockaddr_in from; //Used to display the address of the connection peer
    socklen_t from_len = sizeof(from);
    msgsock = accept(sock, (struct sockaddr*)&from, &from_len); //get the socket address of the connection peer

    //Reset the totalLen and checkSum so that it doesn't carry over between different connections
    totalLen = 0;
    checkSum = 0;

    if (msgsock == -1)
    {
      perror("accept");
    }
    else
    {
      inet_ntop(from.sin_family, &from.sin_addr, buf, sizeof(buf));
      //Output the address and port number that the server connected to
      std::cout << "Accepted connection from '" << buf << "', port " << ntohs(from.sin_port) << std::endl;

      //Start accpeting connections
      do 
      {
	//Read the data sent from the client
        if ((len = read(msgsock, buf, sizeof(buf)-1)) < 0)
        {
          perror("reading stream meesage");
        }
	//EOF so end the connection
        if (len == 0)
        {
	  std::cerr << "Ending connection" << std::endl;
        }
        else
        {
	  //Add the length to the total length
          totalLen += len;

	  //Sum the bits together to get the checkSum
	  for (int i = 0; i < len; i++)
	  {
	    checkSum += (uint8_t)buf[i];
	  }
          buf[len] = '\0';
	}
      } while (len != 0);
      
      //Format a printable string and then extract a C string from it
      std::ostringstream os;
      os << "Sum: " << checkSum << " Len: " << totalLen << "\n";
      std::string str = os.str();
      const char *ch = str.c_str();

      //call safe_write method
      if (safe_write(msgsock, ch, str.length()) < 0)
      {
        perror("writing on stream socket");
      }

      close(msgsock);
    }
  } while (true);
}
