#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>


// Initialize the dispatch center server by creating the server socket, setting up the server address,
// binding the server socket and setting up the server to listen for taxi and customer clients.
void initializeDispatchServer(int *serverSocket, struct sockaddr_in  *serverAddress)
{
  /*serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(serverSocket < 0) {
    printf("*** SERVER ERROR: Could not open socket.\n");
    exit(-1);
  }
    printf("Help\n");
  memset(&serverAddress, 0, sizeof(serverAddress));
    printf("Help2\n");
  serverAddress->sin_family = AF_INET;
      printf("Help3\n");
  serverAddress->sin_addr.s_addr = htonl(INADDR_ANY);
      printf("Help4\n");
  serverAddress->sin_port = htons((unsigned short) SERVER_PORT);
    printf("Help5\n");
*/
}




// Handle client requests coming in through the server socket.  This code should run
// indefinitiely.  It should wait for a client to send a request, process it, and then
// close the client connection and wait for another client.  The requests that may be
// handled are SHUTDOWN (from stop.c), REQUEST_TAXI (from request.c) and UPDATE or
// REQUEST_CUSTOMER (from taxi.c) as follows:

//   SHUTDOWN - causes the dispatch center to go offline.

//   REQUEST_TAXI - contains 2 additional bytes which are the index of the area to be
//                  picked up in and the index of the area to be dropped off in. If
//                  the maximum number of requests has not been reached, a single YES
//                  byte should be sent back, otherwise NO should be sent back.

//   REQUEST_CUSTOMER - contains no additional bytes.  If there are no pending customer
//                      requests, then NO should be sent back.   Otherwise a YES should
//                      be sent back followed by the pickup and dropoff values for the
//                      request that has been waiting the logest in the queue.

//   UPDATE - contains additional bytes representing the taxi's x, y plate, status and
//            dropoff area.  Nothing needs to be sent back to the taxi.

void *handleIncomingRequests(void *d) {
  printf("It goes in here\n");
  DispatchCenter  *dispatchCenter = d;
  dispatchCenter->online=1;

  int                 serverSocket, clientSock;
  struct sockaddr_in  serverAddress, clientAdd;
  int                 status, addrSize, bytesRcv;
  char                buffer[MAX_REQUESTS];
  int numofRequest=0;
  // Initialize the server
  //initializeDispatchServer(&serverSocket, &serverAddress);
  printf("New lands\n");
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(serverSocket < 0) {
    printf("*** SERVER ERROR: Could not open socket.\n");
    exit(-1);
  }
    printf("Help\n");
  memset(&serverAddress, 0, sizeof(serverAddress));
    printf("Help2\n");
  serverAddress.sin_family = AF_INET;
      printf("Help3\n");
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
      printf("Help4\n");
  serverAddress.sin_port = htons((unsigned short) SERVER_PORT);
    printf("Help5\n");
 status= bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if (status<0)
  {printf("Error: can not bind socket."); exit(-1);}
  status=listen(serverSocket,MAX_REQUESTS );
  if (status<0){
    printf("Error, cannot not listen on socket");
    exit(-1);
  }


// Go into infinite loop to talk to client
    while (1) {
      addrSize=sizeof(clientAdd);
      clientSock= accept(serverSocket, (struct sockaddr*) &clientAdd, &addrSize);
      if (clientSock <0){printf("Server error, can't accept incomeing client\n"); exit(-1);}
      bytesRcv=recv(clientSock, buffer, sizeof(buffer),0);
    if(strlen(dispatchCenter->requests)<=MAX_REQUESTS){
          if (buffer[0]==SHUTDOWN)
          {
            dispatchCenter->online=0;
            printf("the sim will shut down dnow\n");
            close (serverSocket);
            break;

          }
          else if (buffer[0]=REQUEST_CUSTOMER)
          {
                printf("Ting goes scrack\n");
                buffer[0]=YES;
                buffer[1]=dispatchCenter->requests[0].pickupLocation;
                buffer[2]=dispatchCenter->requests[0].dropoffLocation;
                for (int i=1; i<strlen(dispatchCenter->requests); i++)
                {
                  printf("hello\n");
                  dispatchCenter->requests[i-1]=dispatchCenter->requests[i];
                }
                printf("sedning back %d YES", buffer[0]);
                send(clientSocket, buffer, strlen(buffer), 0);


          }
          else if (buffer[0]=UPDATE)
          {
            for (int c=0; c< dispatchCenter->numTaxis; c++)
            {
              if (dispatchCenter->taxis[c]=buffer[1])
              {
                dispatchCenter->taxis[c]->x=buffer[2];
                dispatchCenter->taxis[c]->y=buffer[3];
                dispatchCenter->taxis[c]->status=buffer[4];
                dispatchCenter->taxis[c]->dropoffArea=buffer[5];
                printf("recorded");
                break;
              }
            }

          }
          else if (buffer[0]=REQUEST_TAXI)
          {
            dispatchCenter->requests[numofRequest].pickupLocation=buffer[1];
            dispatchCenter->requests[numofRequest].dropoffLocation=buffer[2];
            numofRequest+=1;

          }


      }
        usleep(5000);
    }



  }
