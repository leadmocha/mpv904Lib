#ifndef  MPV904LIB_H
#define  MPV904LIB_H

#define MPV904_BASE_ADDR  0xFFF800
#define MPV904_MAX_BOARDS 10 /* number of boards supported */
#define MPV904_NCHAN  16 /* Number of channels per MPV904 board */


struct mpv904_struct {
  /* Base address if 0xFFF800, increments of 0x2 */
  /* 0x000 */ volatile unsigned short out[MPV904_NCHAN]; /* 0xFFF800 to 0xFFF81E */
};

/* Define prototypes */
int mpv904Init(unsigned int addr, unsigned int addr_inc, int nmpv, unsigned int iFlag);
int mpv904ResetDAC(int id, int ch);
int mpv904SetDAC(int id, int ch, unsigned int dac);
int mpv904SetOutputVoltage(int id, int ch, float voltage);
int mpv904SetVoltageRange(unsigned int flag);

/* A utility function to specify a voltage directly */
int mpv904SetVoltage(unsigned int ch, float voltage);


#endif //MPV904LIB_H
