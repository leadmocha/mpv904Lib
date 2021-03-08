
#define _GNU_SOURCE

#define DEVEL

#ifdef VXWORKS
#include <vxWorks.h>
#include <sysLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <intLib.h>
#include <iv.h>
#include <semLib.h>
#include <vxLib.h>
#include "vxCompat.h"
#include "../jvme/jvme.h"
#else
#include <sys/prctl.h>
#include <unistd.h>
#include "jvme.h"
#endif
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "tiLib.h"
#define _GNU_SOURCE

#define DEVEL

#ifdef VXWORKS
#include <vxWorks.h>
#include <sysLib.h>
#include <logLib.h>
#include <taskLib.h>
#include <intLib.h>
#include <iv.h>
#include <semLib.h>
#include <vxLib.h>
#include "vxCompat.h"
#include "../jvme/jvme.h"
#else
#include <sys/prctl.h>
#include <unistd.h>
#include "jvme.h"
#endif
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "tiLib.h"
#include "mpv904Lib.h"

/* Define global variables */
int nmpv904 = 0;			/* Number of MPV904s in Crate */
unsigned int mpv904A32Base = 0x08000000;	/* Minimum VME A32 Address for use by MPV904s */
unsigned long mpv904A32Offset = 0x08000000;	/* Difference in CPU A32 Base - VME A32 Base */
unsigned long mpv904A24Offset = 0x0;	/* Difference in CPU A24 Base - VME A24 Base */
volatile struct mpv904_struct *mpv904p[(MPV904_MAX_BOARDS + 1)];	/* pointers to MPV904 memory map */


/* Initialize the MPV904 */
STATUS mpv904Init(UINT32 addr, UINT32 addr_inc, int nmpv)
{
  int res; //< Variable to hold result of functions
  int rdata; //< Response of data from VME reads
  int ii; //< Helper variable for loops
  int errFlag = 0; ///< Error flag for debuging
  unsigned long laddr; //< Map of MPV904 to system memory
  volatile struct mpv904_struct *mpv904;

  /* Check for valid address */
  if (addr == 0) {
    printf("mpv904Init: ERROR: Must specify a Bus (VME-based A24) address for MPV904\n");
    return (ERROR);
  } else if (addr > 0x00ffffff) { /* A24 Addressing */
    printf("mpv904Init: ERROR: A32 Addressing not allowed for the MPV904\n");
    return (ERROR);
  } else {
    if(addr_inc == 0 || nmpv <= 0) { /* Assume only one MPV needs to be initialized */
      nmpv = 1;
    }

    /* get the MPV904 address */
#ifdef VXWORKS
    res = sysBusToLocalAdrs(0x39, (char *) addr, (char **) &laddr);
    if (res != 0) {
      printf("mpv904Init: ERROR in sysBusToLocalAdrs(0x39,0x%x,&laddr) \n",addr);
      return (ERROR);
    }
#else
    res = vmeBusToLocalAdrs(0x39, (char *) (unsigned long) addr,
        (char **) (unsigned long) &laddr);
    if (res != 0) {
      printf("mpv904Init: ERROR in vmeBusToLocalAdrs(0x39,0x%x,&laddr) \n",addr);
      return (ERROR);
    }
#endif
    mpv904A24Offset = laddr - addr;
  }

  /* Begin setup process for MPVs */
  nmpv904 = 0;
  for(ii = 0; ii < nmpv; ii++) {
    mpv904 = (struct mpv904_struct *)(laddr + ii * addr_inc);
    /* Check if the board exists at that address */
#ifdef VXWORKS
    res = vxMemProbe((char *) &(mpv904->out[0]), VX_READ, 4, (char *) &rdata);
#else
    res = vmeMemProbe((char *) &(mpv904->out[0]), 4, (char *) &rdata);
#endif
    if( res < 0 ) {
#ifdef VXWORKS
      printf("mpv904Init: ERROR: No addressable board at addr=0x%x\n", (UINT32) mpv904);
#else
      printf
        ("mpv904Init: ERROR: No addressable board at VME (Local) addr=0x%x (0x%lx)\n",
         (UINT32) addr + ii * addr_inc, (unsigned long) mpv904);
#endif
      errFlag = 1;
      break;
    } else {
      mpv904p[ii] = (struct mpv904_struct *)(laddr +ii *addr_inc);
    }
    nmpv904++;
#ifdef VXWRKS
    printf("Inialized MPV %d at address 0x%08x\n",ii, mpv904p[ii]);
#else
    printf("Inialized MPV %d at VME (Local) address 0x%08x (0x%lx)\n",ii,
      (unsigned int)((unsigned long) mpv904p[ii] - mpv904A24Offset),
      (unsigned long)mpv904p[ii]);
#endif
  }

  return (OK);
}
