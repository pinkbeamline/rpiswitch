#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <unistd.h>
#include "epicsThread.h"
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <wiringPi.h>

#define N_INPUTS 4
#define N_OUTPUTS 8

// Global variables
epicsThreadId tid[5];
int input_pins[] = { 2,3,4,5 };
int output_pins[] = { 16,15,13,14,27,26,21,22 };
int inputs[N_INPUTS];

// write output function
static long writeaddr(subRecord *precord){
  int addr = (int)precord->a;
  int val;

  if(precord->val==0){
    val=0;
  }else{
    val=1;
  }

  if( (addr>=0) && (addr<N_OUTPUTS) ){
    digitalWrite(output_pins[addr], val);
    //printf("Addr: %d\tPin: %d\tVal: %d\n",addr,output_pins[addr],val);
  }else{
    printf("Output address not valid\n");
  }

  return 0;
}

// read input funtion
static long readaddr(subRecord *precord){
  int addr = (int)precord->a;

  if( (addr>=0) && (addr<N_INPUTS) ){
    precord->val=inputs[addr];
  }else{
    printf("Input address not valid\n");
  }

  return 0;
}

static void gpiomonitor(void *ctx){
  while(1){
    for(int i=0; i<N_INPUTS; i++){
      inputs[i]=digitalRead(input_pins[i]);
    }
    delay(100); //msec
  }
}

static long startserver(subRecord *precord){
  // setup GPIO
  if (wiringPiSetupGpio () == -1){
    printf("Failed setting up GPIO. Aborting.");
    return 1;
  }

  // setup input pins
  for(int i=0; i<N_INPUTS; i++){
    pinMode (input_pins[i], INPUT);
    pullUpDnControl (input_pins[i], PUD_OFF);
  }

  // setup output pins
  for(int i=0; i<N_OUTPUTS; i++){
    pinMode (output_pins[i], OUTPUT);
  }

  // reset input array
  for(int i=0; i<N_INPUTS; i++){
    inputs[i]=0;
  }

  tid[0]=epicsThreadCreate("GPIO_MONITOR", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), gpiomonitor, 0);
  if (!tid[0]){
    printf("epicsThreadCreate [0] failed\n");
  }else{
    printf("[GPIO Monitor]: Running...\n");
  }

  return 0;
}

// switch connection
static long cswitch(subRecord *precord){
  int addr = (int)precord->a;
  int val = (int)precord->val;
  int offset, channel;

  if(addr==1){
    offset=4;
  }else{
    offset=0;
  }

  if( (val>=0)&&(val<=4) ){
    channel=val-1+offset;
    for(int i=0; i<4; i++){
      digitalWrite(output_pins[i+offset], 0);
    }
    if(val>0) digitalWrite(output_pins[channel], 1);
  }

  return 0;
}

epicsRegisterFunction(startserver);
epicsRegisterFunction(readaddr);
epicsRegisterFunction(writeaddr);
epicsRegisterFunction(cswitch);
