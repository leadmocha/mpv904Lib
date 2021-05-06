/*
 * File:
 *    mpv904SetDac <output> <dac value>
 *
 * Description:
 *    Set the DAC for specified output
 *
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jvme.h"
#include "tiLib.h"
#include "mpv904Lib.h"

#define MPV904_ADDR (14 << 19)
void usage();
char progname[256];

int
main(int argc, char *argv[])
{

  int stat;
  int res = 0; //< result from vmeRead
  int output = -1;
  unsigned int dac = 0;

  strncpy(progname, argv[0], 256);

  if(argc != 3) {
      usage();
      exit(-1);
    }

  output = atoi(argv[1]);
  dac = strtol(argv[2], NULL, 10);

  printf("   Setting output %d to 0x%x (%d)\n",
	 output, dac, dac);

  if(vmeOpenDefaultWindows() != OK) {
    goto CLOSE;
  }

  printf("   Initializing mpv904 (at 0x%08x)\n",
	 MPV904_ADDR);
  stat = mpv904Init(MPV904_ADDR,0,0,0);

  if(stat != OK) {
    printf("Error initializing mpv904\n");
  } else {

    mpv904SetDAC(0, output, dac);

  }

 CLOSE:
  vmeCloseDefaultWindows();


  exit(0);
}

void
usage()
{
  printf("\n");

  printf("Usage:\n");
  printf("%s [OUTPUT] [DAC]\n", progname);
  printf("Set mpv904 (VME address 0x%08x) OUTPUT dac to DAC.\n", MPV904_ADDR);
  printf("\n");
  printf("OUTPUT range : 0 - 15\n");
  printf("   DAC range : 0 - 4095\n");
  printf("\n\n");

}

/*
  Local Variables:
  compile-command: "make mpv904SetDac"
  End:
 */
