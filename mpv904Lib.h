#ifndef  DACLIB_H
#define  DACLIB_H

#define MPV904_BASE_ADDR  0xFFF800
#define MPV904_MAX_BOARDS 10 /* number of boards supported */


struct mpv904_struct {
  /* Base address if 0xFFF800, increments of 0x2 */
  /* 0x000 */ volatile unsigned int out[16]; /* 0xFFF800 to 0xFFF81E */
};

/* Define prototypes */
int mpv904Init(unsigned int addr, unsigned int addr_inc, int nmpv);


#endif //DALIB_H
