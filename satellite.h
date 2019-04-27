#ifndef SATELLITE_H
#define SATELLITE_H

#include "almanac/cls_Alm.h"

#define _MAX_GPS_SAT_ (32)
#define _MAX_GLN_SAT_ (24)
#define _MAX_SAT_      (_MAX_GPS_SAT_ + _MAX_GLN_SAT_)



extern ALM_GPS almGps[_MAX_GPS_SAT_];

void read_almanac (char fname_alm_gps[], char fname_alm_gln[]);

typedef struct
{
  char sys;
  int num;
} SAT_ID;



#endif // SATELLITE_H
