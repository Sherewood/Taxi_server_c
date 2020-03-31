#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"


void main() {
  int                 clientSocket;
struct sockaddr_in    Address;
int                 status, bytesRcv;

char                buffer[80];   // stores sent and received data
printf("stop1 \n");
// Create socket
clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if (clientSocket < 0) {
  printf("*** CLIENT ERROR: Could open socket.\n");
  exit(-1);
}
printf("stop2\n");

// Setup address
memset(&Address, 0, sizeof(Address));
printf("HelloWORLDS\n");
Address.sin_family = AF_INET;
printf("AVAdaCKADS\n");
Address.sin_addr.s_addr = inet_addr(SERVER_IP);
printf("This is new\n");
Address.sin_port = htons((unsigned short) SERVER_PORT);
printf("stop3\n");
// Connect to server
status = connect(clientSocket, (struct sockaddr *) &Address, sizeof(Address));
if (status < 0) {
  printf("*** CLIENT ERROR: Could not connect.\n");
  exit(-1);
}

  buffer[0]=SHUTDOWN;
  printf("CLIENT: Sending %d to server.\n", SHUTDOWN);
  send(clientSocket, buffer, strlen(buffer), 0);

close(clientSocket);  // Don't forget to close the socket !
printf("CLIENT: Shutting down.\n");
  // Contact the dispatch center and ask it to shut down
  // Create socket

}
