#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "simulator.h"

void main() {
  // Set up the random seed
  srand(time(NULL));

  while(1) {
    int PI= rand()%6;
    int DO= rand()%6;
    char customer[100];
    sprintf(customer, "./customer %d %d", PI, DO);
    system(customer);
    // add code here
    // ...
  //  system(./customer);

    usleep(250000);   /// do not remove this
  }
}
//system("./customer 0 5");
