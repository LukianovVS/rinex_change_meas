#ifndef SATELLITE_H
#define SATELLITE_H

#include "almanac/cls_Alm.h"

#define _MAX_GPS_SAT_ (32)
#define _MAX_GLN_SAT_ (24)
#define _MAX_SAT_      (_MAX_GPS_SAT_ + _MAX_GLN_SAT_)



extern ALM_GPS almGps[_MAX_GPS_SAT_];
extern int LS;


typedef struct
{
  char sys;
  int num;
} SAT_ID;

typedef struct
{
  int year;
  int month;
  int day;
  int h;
  int m;
  double sec;
} TIME;


typedef struct
{
  int week;
  double tow;
} TIME_GPS;

typedef struct
{
  int N4;
  int N0;
  double sec;
} TIME_GLN;


void utc2gps(TIME_GPS *tgps, TIME tutc, const int LS);

void read_almanac (char fname_alm_gps[], char fname_alm_gln[]);
void calc_dr(double &dR, double &dL1, double dL2, const SAT_ID sat,  TIME_GPS t_gps,  TIME_GLN t_gln, const double xyz_receiver[], const double dxyz[]);


#endif // SATELLITE_H
