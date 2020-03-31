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

// This represents a customer request.  There should be two command line arguments representing the
// pickup location and dropoff location.  Each should be a number from 0 to the NUM_CITY_AREAS.
void main(int argc, char *argv[]) {

  int PI=atoi(argv[1]);
  int DO=atoi(argv[2]);
  if (PI!=NULL || DO!=NULL){
    int                 clientS;
    struct sockaddr_in    address;
    int                 status, bytesRcv;

    char                buffer[80];
    clientS = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientS< 0) {
      printf("*** CLIENT ERROR: Could open socket.\n");
      exit(-1);
    }
    printf("***CONNECTED To Customer\n");
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons((unsigned short) SERVER_PORT);
    // Connect to server
    status = connect(clientS, (struct sockaddr *) &address, sizeof(address));
    if (status < 0) {
      printf("*** CLIENT ERROR: hgj Could not connect.\n");
      exit(-1);
    }
    if (PI<=NUM_CITY_AREAS || PI>=0 && DO<=NUM_CITY_AREAS ||DO>=0){
      printf("You entered good coordinates %d and %d\n", PI, DO);
    buffer[0]=REQUEST_TAXI;
    buffer[1]=AREA_NAMES[PI];
    buffer[2]=AREA_NAMES[DO];
    send(clientS, buffer, strlen(buffer), 0);
    }
    else{
      printf("Your request was invalid or the server is full\n" );
    }
  }
  else{
    printf("you have not entered enough details");

  }
}
