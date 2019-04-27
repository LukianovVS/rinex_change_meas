#ifndef SATELLITE_H
#define SATELLITE_H

#include "almanac/cls_Alm.h"

#define _MAX_GPS_SAT_ (32)


extern ALM_GPS almGps[_MAX_GPS_SAT_];

void read_almanac (char fname_alm_gps[], char fname_alm_gln[]);


#endif // SATELLITE_H
