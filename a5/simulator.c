#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "simulator.h"

#include "taxi.c"
#include "display.c"
#include "dispatchCenter.c"

int main() {
  DispatchCenter     ottawaDispatch;


  srand(time(NULL));
  ottawaDispatch.numTaxis = 0;
  ottawaDispatch.numRequests = 0;
  pthread_t t1, t2;
  for (int i=0; i<1; i++)
  {

  Taxi *TAXI;
  TAXI= malloc(sizeof(Taxi));
  ottawaDispatch.taxis[i]= TAXI;
  TAXI->plateNumber=0;
  int randum= rand()%6;
  TAXI->currentArea= AREA_NAMES[randum];
  TAXI->pickupArea=UNKNOWN_AREA;
  TAXI->dropoffArea=UNKNOWN_AREA;
  TAXI->x=20;
  TAXI->y=10;
  printf("KKKK\n");
  TAXI->status=AVAILABLE;
  pid_t pid= fork();
  //printf("%d \n", childPID);
  if(pid==0)
    {
    printf("MASAD\n");
    runTaxi(TAXI);
    return 0;

    }
  else{
    printf("ASD\n");
    TAXI->pID=pid;
      }
  }
  printf("ASDASDSADASDSAD\n");
  pthread_create(&t1, NULL,handleIncomingRequests,&ottawaDispatch);
  pthread_create(&t2, NULL ,showSimulation, &ottawaDispatch);
  pthread_join(t1, NULL);

  printf("Simulation complete.\n");
}
