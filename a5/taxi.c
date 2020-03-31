#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// The dispatch center server connection ... when made
int                 clientSocket;  // client socket id
struct sockaddr_in  clientAddress; // client address



// Set up a client socket and connect to the dispatch center server.  Return -1 if there was an error.
int connectToDispatchCenter(int *sock,  struct sockaddr_in *address) {


  return 0;
}


int ETA(Taxi taxi, char *whichone)
{
  int Delta;
  int timeforp= (TIME_ESTIMATES[AREA_X_LOCATIONS[taxi.currentArea]][AREA_Y_LOCATIONS[taxi.currentArea]])-(TIME_ESTIMATES[AREA_X_LOCATIONS[taxi.pickupArea]][AREA_Y_LOCATIONS[taxi.pickupArea]]);
  int timeford=(TIME_ESTIMATES[AREA_X_LOCATIONS[taxi.currentArea]][AREA_Y_LOCATIONS[taxi.currentArea]])-(TIME_ESTIMATES[AREA_X_LOCATIONS[taxi.dropoffArea]][AREA_Y_LOCATIONS[taxi.dropoffArea]]);
  if (whichone=="x")
  {
    if (taxi.status=PICKING_UP)
    {
      Delta= (AREA_X_LOCATIONS[taxi.pickupArea]- AREA_X_LOCATIONS[taxi.currentArea])/timeforp;

    }
    else
    {
      Delta=(AREA_X_LOCATIONS[taxi.dropoffArea]- AREA_X_LOCATIONS[taxi.currentArea])/timeford;

    }
  }
  else if (whichone=="y")
  {
    if (taxi.status=PICKING_UP)
    {
      Delta= (AREA_Y_LOCATIONS[taxi.pickupArea]- AREA_X_LOCATIONS[taxi.currentArea])/timeforp;
    }
    else
    {
      Delta=(AREA_Y_LOCATIONS[taxi.dropoffArea]- AREA_X_LOCATIONS[taxi.currentArea])/timeford;

    }
  }
  return Delta;
}


// This code runs the taxi forever ... until the process is killed
void runTaxi(Taxi *taxi) {
  // Copy the data over from this Taxi to a local copy
  Taxi   thisTaxi;
  thisTaxi.plateNumber = taxi->plateNumber;
  thisTaxi.currentArea = taxi->currentArea;
  thisTaxi.x = taxi->x;
  thisTaxi.y = taxi->y;
  thisTaxi.status = AVAILABLE;
  thisTaxi.pickupArea = UNKNOWN_AREA;
  thisTaxi.dropoffArea = UNKNOWN_AREA;
  thisTaxi.eta = 0;
  printf("It gets into Taxi");

  // Go into loop to commuincate with server now
  while (1) {
    int                 clientSocket;
    struct sockaddr_in    address;
    int                 status, bytesRcv;

    char                buffer[80];
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0) {
      printf("*** CLIENT ERROR: Could open socket.\n");
      exit(-1);
    }
    printf("***CONNECTED To taxi\n");
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    printf("This is new\n");
    address.sin_port = htons((unsigned short) SERVER_PORT);
    // Connect to server
    status = connect(clientSocket, (struct sockaddr *) &address, sizeof(address));
    if (status < 0) {
      printf("*** CLIENT ERROR: hgj Could not connect.\n");
      exit(-1);
    }
    buffer[0]=REQUEST_CUSTOMER;
    send(clientSocket, buffer, strlen(buffer), 0);
    printf("sent %d\n",buffer[0]);
    bytesRcv=recv(clientSocket, buffer, sizeof(buffer),0);
    if (buffer[0]=YES){
      if (buffer[1]=thisTaxi.currentArea)
        {
        printf("ready to rock and roll\n");

        thisTaxi.status=DROPPING_OFF;
        thisTaxi.dropoffArea=buffer[2];
        thisTaxi.x+=ETA(thisTaxi, "x");
        thisTaxi.y+=ETA(thisTaxi,"y");
        thisTaxi.eta=(thisTaxi.x,thisTaxi.y);
        buffer[3]=thisTaxi.x;
        buffer[4]=thisTaxi.y;
        buffer[5]=thisTaxi.eta;
        buffer[0]=UPDATE;
        send(clientSocket, buffer, strlen(buffer), 0);


        }
        else
        {

          printf("ready to roll\n");
          thisTaxi.status=PICKING_UP;
          thisTaxi.dropoffArea=buffer[2];
          thisTaxi.x+=ETA(thisTaxi, "x");
          thisTaxi.y+=ETA(thisTaxi,"y");
          thisTaxi.eta=(thisTaxi.x,thisTaxi.y);
          buffer[3]=thisTaxi.x;
          buffer[4]=thisTaxi.y;
          buffer[5]=thisTaxi.eta;
          buffer[0]=UPDATE;
          send(clientSocket, buffer, strlen(buffer), 0);

        }
      }
    }
    // Contact the dispatch center and ask it to shut down
    usleep(500000);  // A delay to slow things down a little
  }
