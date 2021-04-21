/*
 * File:
 *    tiLibTest.c
 *
 * Description:
 *    Test Vme TI interrupts with GEFANUC Linux Driver
 *    and TI library
 *
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jvme.h"
#include "tiLib.h"
#include "mpv904Lib.h"
/* #include "remexLib.h" */

DMA_MEM_ID vmeIN,vmeOUT;
extern DMANODE *the_event;
extern unsigned int *dma_dabufp;
volatile struct mpv904_struct *mpv904p[(MPV904_MAX_BOARDS + 1)];
extern int tiA32Base;

#define BLOCKLEVEL 30

#define DO_READOUT


int 
main(int argc, char *argv[]) {

  int stat;
  int res = 0; //< result from vmeRead

  printf("\nMPV904 Lib Test\n");
  printf("----------------------------\n");
/*   remexSetCmsgServer("dafarm28"); */
/*   remexInit(NULL,1); */

  //printf("Size of DMANODE    = %d (0x%x)\n",sizeof(DMANODE),sizeof(DMANODE));
  //printf("Size of DMA_MEM_ID = %d (0x%x)\n",sizeof(DMA_MEM_ID),sizeof(DMA_MEM_ID));

  vmeOpenDefaultWindows();

  /* Setup Address and data modes for DMA transfers
   *   
   *  vmeDmaConfig(addrType, dataType, sstMode);
   *
   *  addrType = 0 (A16)    1 (A24)    2 (A32)
   *  dataType = 0 (D16)    1 (D32)    2 (BLK32) 3 (MBLK) 4 (2eVME) 5 (2eSST)
   *  sstMode  = 0 (SST160) 1 (SST267) 2 (SST320)
   */
  vmeDmaConfig(2,5,1);

  /* INIT dmaPList */

  dmaPFreeAll();
  vmeIN  = dmaPCreate("vmeIN",10240,50,0);
  vmeOUT = dmaPCreate("vmeOUT",0,0,0);
    
  dmaPStatsAll();

  dmaPReInitAll();





  printf("Initializing mpv904\n");
  stat = mpv904Init(14<<19,0,0,0);
  if(stat != OK) {
    printf("Error initializing mpv904\n");
  } else {
    printf("Finished initializing mpv904\n");
    printf("Setting out[0] to 0xFFF\n");
    vmeWrite32(&(mpv904p[0]->out[0]),0xFFF);
    res = vmeRead32(&(mpv904p[0]->out[0]));
    printf("Reading out[0] %d\n", res);
    printf("Setting out[0] to 0xF0F\n");
    vmeWrite32(&(mpv904p[0]->out[0]),0xF0F);
    res = vmeRead32(&(mpv904p[0]->out[0]));
    printf("Reading out[0] %d\n", res);
  }

  dmaPFreeAll();
  vmeCloseDefaultWindows();


  exit(0);
}

